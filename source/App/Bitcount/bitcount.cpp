/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2026, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <map>
#include "CommonLib/CommonDef.h"
#include "DecoderLib/NALread.h"
#include "VLCReader.h"
#if ENABLE_TRACING
#include "CommonLib/dtrace_next.h"
#endif

#define PRINT_NALUS 1

using PocBits = std::map<int, int>;   // map of poc to bits

class ParcatHLSyntaxReader : public VLCReader
{
public:
  void parsePictureHeaderUpToPoc(ParameterSetManager* parameterSetManager);
  bool parsePictureHeaderInSliceHeaderFlag(ParameterSetManager* parameterSetManager);
};

bool ParcatHLSyntaxReader::parsePictureHeaderInSliceHeaderFlag(ParameterSetManager* parameterSetManager)
{
  uint32_t code;
  xReadFlag(code, "sh_picture_header_in_slice_header_flag");
  return (code == 1);
}

void ParcatHLSyntaxReader::parsePictureHeaderUpToPoc(ParameterSetManager* parameterSetManager)
{
  uint32_t code;

  PPS* pps = nullptr;
  SPS* sps = nullptr;

  uint32_t tmp;
  xReadFlag(tmp, "ph_gdr_or_irap_pic_flag");
  xReadFlag(code, "ph_non_ref_pic_flag");
  if (tmp)
  {
    xReadFlag(code, "ph_gdr_pic_flag");
  }
  xReadFlag(code, "ph_inter_slice_allowed_flag");
  if (code)
  {
    xReadFlag(code, "ph_intra_slice_allowed_flag");
  }
  // parameter sets
  xReadUvlc(code, "ph_pic_parameter_set_id");
  pps = parameterSetManager->getPPS(code);
  CHECK(pps == 0, "Invalid PPS");
  sps = parameterSetManager->getSPS(pps->getSPSId());
  CHECK(sps == 0, "Invalid SPS");
  return;
}

/**
 Find the beginning and end of a NAL (Network Abstraction Layer) unit in a byte buffer containing H264 bitstream data.
 @param[in]   buf        the buffer
 @param[in]   size       the size of the buffer
 @param[out]  nal_start  the beginning offset of the nal
 @param[out]  nal_end    the end offset of the nal
 @return                 the length of the nal, or 0 if did not find start of nal, or -1 if did not find end of nal
 */
// DEPRECATED - this will be replaced by a similar function with a slightly different API
int find_nal_unit(const uint8_t* buf, int size, int* nalStart, int* nalEnd)
{
  int i;
  // find start
  *nalStart = 0;
  *nalEnd   = 0;

  i = 0;
  while (   //( next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001 )
    (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01)
    && (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0 || buf[i + 3] != 0x01))
  {
    i++;   // skip leading zero
    if (i + 4 >= size)
    {
      return 0;
    }   // did not find nal start
  }

  if (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01)   // ( next_bits( 24 ) != 0x000001 )
  {
    i++;
  }

  if (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01)
  { /* error, should never happen */
    return 0;
  }
  i += 3;
  *nalStart = i;

  while (   //( next_bits( 24 ) != 0x000000 && next_bits( 24 ) != 0x000001 )
    i + 3 < size && (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0)
    && (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0x01))
  {
    i++;
    // FIXME the next line fails when reading a nal that ends exactly at the end of the data
  }

  if (i + 3 == size)
  {
    *nalEnd = size;
  }
  else
  {
    *nalEnd = i;
  }

  return (*nalEnd - *nalStart);
}

const bool verbose = false;

const char* NALU_TYPE[] = {
  "NAL_UNIT_CODED_SLICE_TRAIL",
  "NAL_UNIT_CODED_SLICE_STSA",
  "NAL_UNIT_CODED_SLICE_RADL",
  "NAL_UNIT_CODED_SLICE_RASL",
  "NAL_UNIT_RESERVED_VCL_4",
  "NAL_UNIT_RESERVED_VCL_5",
  "NAL_UNIT_RESERVED_VCL_6",
  "NAL_UNIT_CODED_SLICE_IDR_W_RADL",
  "NAL_UNIT_CODED_SLICE_IDR_N_LP",
  "NAL_UNIT_CODED_SLICE_CRA",
  "NAL_UNIT_CODED_SLICE_GDR",
  "NAL_UNIT_RESERVED_IRAP_VCL11",
  "NAL_UNIT_RESERVED_IRAP_VCL12",
  "NAL_UNIT_DPS",
  "NAL_UNIT_VPS",
  "NAL_UNIT_SPS",
  "NAL_UNIT_PPS",
  "NAL_UNIT_PREFIX_APS",
  "NAL_UNIT_SUFFIX_APS",
  "NAL_UNIT_PH",
  "NAL_UNIT_ACCESS_UNIT_DELIMITER",
  "NAL_UNIT_EOS",
  "NAL_UNIT_EOB",
  "NAL_UNIT_PREFIX_SEI",
  "NAL_UNIT_SUFFIX_SEI",
  "NAL_UNIT_FD",
  "NAL_UNIT_RESERVED_NVCL26",
  "NAL_UNIT_RESERVED_NVCL27",
  "NAL_UNIT_UNSPECIFIED_28",
  "NAL_UNIT_UNSPECIFIED_29",
  "NAL_UNIT_UNSPECIFIED_30",
  "NAL_UNIT_UNSPECIFIED_31",
};

int calcPoc(int pocLsb, int prevTid0POC, int getBitsForPOC, int nalu_type)
{
  int prevPoc    = prevTid0POC;
  int maxPocLsb  = 1 << getBitsForPOC;
  int prevPocLsb = prevPoc & (maxPocLsb - 1);
  int prevPocMsb = prevPoc - prevPocLsb;
  int pocMsb;
  if ((pocLsb < prevPocLsb) && ((prevPocLsb - pocLsb) >= (maxPocLsb / 2)))
  {
    pocMsb = prevPocMsb + maxPocLsb;
  }
  else if ((pocLsb > prevPocLsb) && ((pocLsb - prevPocLsb) > (maxPocLsb / 2)))
  {
    pocMsb = prevPocMsb - maxPocLsb;
  }
  else
  {
    pocMsb = prevPocMsb;
  }

  return pocMsb + pocLsb;
}

PocBits processBuffer(const std::vector<uint8_t>& v)
{
  const uint8_t* p   = v.data();
  const uint8_t* buf = v.data();
  int            sz  = (int) v.size();
  int            nalStart, nalEnd;
  int            off = 0;

  PocBits out;

  int bitsForPoc = 8;

  int pendingBits = 0;
  int currentPoc  = -1;

  while (find_nal_unit(p, sz, &nalStart, &nalEnd) > 0)
  {
    if (verbose)
    {
      printf("!! Found NAL at offset %lld (0x%04llX), size %lld (0x%04llX) \n", (long long int) (off + (p - buf)),
             (long long int) (off + (p - buf)), (long long int) (nalEnd - nalStart),
             (long long int) (nalEnd - nalStart));
    }

    p += nalStart;

    const int bitsInNalu = 8 * nalEnd;

    std::vector<uint8_t> nalu(p, p + nalEnd - nalStart);
    int                  naluType = nalu[1] >> 3;
#if ENABLE_TRACING
    printf("NALU Type: %d (%s)\n", naluType, NALU_TYPE[naluType]);
#endif
    int pocLsb = -1;

    HLSyntaxReader             HLSReader;
    static ParameterSetManager parameterSetManager;
    ParcatHLSyntaxReader       parcatHLSReader;
    InputNALUnit               inputNalu;
    std::vector<uint8_t>&      naluBytes = inputNalu.getBitstream().getFifo();
    naluBytes                            = nalu;
    read(inputNalu);

    if (inputNalu.m_nalUnitType == NAL_UNIT_SPS)
    {
      SPS* sps = new SPS();
      HLSReader.setBitstream(&inputNalu.getBitstream());
      HLSReader.parseSPS(sps);
      parameterSetManager.storeSPS(sps, inputNalu.getBitstream().getFifo());
    }

    if (inputNalu.m_nalUnitType == NAL_UNIT_PPS)
    {
      PPS* pps = new PPS();
      HLSReader.setBitstream(&inputNalu.getBitstream());
      HLSReader.parsePPS(pps);
      parameterSetManager.storePPS(pps, inputNalu.getBitstream().getFifo());
    }

    if (naluType == NAL_UNIT_CODED_SLICE_IDR_W_RADL || naluType == NAL_UNIT_CODED_SLICE_IDR_N_LP)
    {
      currentPoc = 0;
    }

    if (inputNalu.m_nalUnitType == NAL_UNIT_PH || naluType <= NAL_UNIT_RESERVED_IRAP_VCL_11)
    {
      parcatHLSReader.setBitstream(&inputNalu.getBitstream());
      bool computePoc = false;
      if (inputNalu.m_nalUnitType == NAL_UNIT_PH)
      {
        computePoc = true;
      }
      else
      {
        computePoc = parcatHLSReader.parsePictureHeaderInSliceHeaderFlag(&parameterSetManager);
      }
      if (computePoc)
      {
        // beginning of picture header parsing
        parcatHLSReader.parsePictureHeaderUpToPoc(&parameterSetManager);
        int numBitsUpToPocLsb = parcatHLSReader.getBitstream()->getNumBitsRead();
        int offset            = numBitsUpToPocLsb;

        int      byteOffset = offset / 8;
        int      hiBits     = offset % 8;
        uint16_t data       = (nalu[byteOffset] << 8) | nalu[byteOffset + 1];
        int      lowBits    = 16 - hiBits - bitsForPoc;
        pocLsb              = (data >> lowBits) & 0xff;
        currentPoc          = calcPoc(pocLsb, currentPoc, bitsForPoc, naluType);
      }

      if (out.find(currentPoc) != out.end())
      {
        out[currentPoc] += bitsInNalu + pendingBits;
      }
      else
      {
        out[currentPoc] = bitsInNalu + pendingBits;
      }
      pendingBits = 0;
    }
    else if (inputNalu.m_nalUnitType == NAL_UNIT_SUFFIX_APS || inputNalu.m_nalUnitType == NAL_UNIT_SUFFIX_SEI)
    {
      out[currentPoc] += bitsInNalu;
    }
    else
    {
      // Don't know yet the associate POC value for this NAL, so accumulate to pendingBits and add it when POC can be
      // determined
      pendingBits += bitsInNalu;
    }

    p += (nalEnd - nalStart);
    sz -= nalEnd;
  }

  size_t sum = 0;
  for (const auto& p: out)
  {
    sum += p.second;
  }
  if (sum != v.size() * 8)
  {
    fprintf(stderr, "Error: total bits counted does not match input size.\n");
    exit(1);
  }
  return out;
}

PocBits processFile(const char* path)
{
  FILE* fdi = fopen(path, "rb");

  if (fdi == nullptr)
  {
    fprintf(stderr, "Error: could not open input file: %s", path);
    exit(1);
  }

  fseek(fdi, 0, SEEK_END);
  long fullSize = ftell(fdi);
  fseek(fdi, 0, SEEK_SET);

  std::vector<uint8_t> v(fullSize);

  size_t sz = fread((char*) v.data(), 1, fullSize, fdi);
  fclose(fdi);

  if (sz != fullSize)
  {
    fprintf(stderr, "Error: input file was not read completely.");
    exit(1);
  }

  return processBuffer(v);
}

int main(int argc, char* argv[])
{
#if ENABLE_TRACING
  std::string tracingFile;
  std::string tracingRule;

  g_trace_ctx = tracing_init(tracingFile, tracingRule);
#endif
  if (argc < 2)
  {
    printf("bitcount version VTM %s\n", VTM_VERSION);
    printf("usage: %s <bitstream1>\n", argv[0]);
    return -1;
  }

  initROM();

  PocBits v = processFile(argv[1]);

  for (const auto& p: v)
  {
    printf("%d,%d\n", p.first, p.second);
  }

#if ENABLE_TRACING
  tracing_uninit(g_trace_ctx);
#endif
}

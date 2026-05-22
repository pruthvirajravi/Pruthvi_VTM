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
#include <cstdio>
#include <fstream>
#include <memory>
#include <string>

#include "CommonLib/version.h"
#include "program_options_lite.h"

#if defined (_WIN32) || defined (WIN32) || defined (_WIN64) || defined (WIN64)
#include <io.h>
#include <fcntl.h>
#endif

typedef std::unique_ptr< unsigned char[] > PayloadPtr;

int convertToPacked( const PayloadPtr& payloadPtrSrc, PayloadPtr& payloadPtrDst, const size_t& samples )
{
  unsigned short* src = reinterpret_cast<unsigned short*>( payloadPtrSrc.get() );
  unsigned char*  dst = payloadPtrDst.get();
  for( size_t s = 0; s < samples; s+=4 )
  {
    uint64_t tmp = (((uint64_t)(src[0] & 0x3FF)) <<  0)
                   + (((uint64_t)(src[1] & 0x3FF)) << 10)
                   + (((uint64_t)(src[2] & 0x3FF)) << 20)
                   + (((uint64_t)(src[3] & 0x3FF)) << 30);

    *dst++ = (unsigned char)((tmp >>  0) & 0xFF);
    *dst++ = (unsigned char)((tmp >>  8) & 0xFF);
    *dst++ = (unsigned char)((tmp >> 16) & 0xFF);
    *dst++ = (unsigned char)((tmp >> 24) & 0xFF);
    *dst++ = (unsigned char)((tmp >> 32) & 0xFF);
    src += 4;
  }

  return 0;
}

int convertToUnpacked( const PayloadPtr& payloadPtrSrc, PayloadPtr& payloadPtrDst, const size_t& samples )
{
  unsigned char*  src = payloadPtrSrc.get();
  unsigned char*  dst = payloadPtrDst.get();

  for( size_t s = 0; s < samples; s += 4 )
  {
    uint64_t tmp = ((uint64_t)src[0] <<  0)
                   | ((uint64_t)src[1] <<  8)
                   | ((uint64_t)src[2] << 16)
                   | ((uint64_t)src[3] << 24)
                   | ((uint64_t)src[4] << 32);

    uint16_t s0 = (uint16_t)(0x03FF & (tmp >>  0));
    uint16_t s1 = (uint16_t)(0x03FF & (tmp >> 10));
    uint16_t s2 = (uint16_t)(0x03FF & (tmp >> 20));
    uint16_t s3 = (uint16_t)(0x03FF & (tmp >> 30));

    // Write 16-bit samples in little-endian byte order
    *dst++ = (unsigned char)( s0       & 0xFF);
    *dst++ = (unsigned char)((s0 >> 8) & 0xFF);
    *dst++ = (unsigned char)( s1       & 0xFF);
    *dst++ = (unsigned char)((s1 >> 8) & 0xFF);
    *dst++ = (unsigned char)( s2       & 0xFF);
    *dst++ = (unsigned char)((s2 >> 8) & 0xFF);
    *dst++ = (unsigned char)( s3       & 0xFF);
    *dst++ = (unsigned char)((s3 >> 8) & 0xFF);
    src += 5;
  }

  return 0;
}

size_t readPayload( std::istream& inStream, PayloadPtr& payloadPtr, const size_t& payloadSize )
{
  char *pcSrc = reinterpret_cast< char* >( payloadPtr.get() );
  inStream.read( pcSrc, payloadSize );
  if( inStream.gcount() > 0 && inStream.gcount() < (std::streamsize)payloadSize )
  {
    fprintf(stderr, "Warning: Could only read %ld bytes of expected %ld bytes\n", (long)inStream.gcount(), (long)payloadSize);
    return inStream.gcount();
  }
  else if( inStream.gcount() != (std::streamsize)payloadSize )
  {
    return 0;
  }
  return payloadSize;
}

size_t getFileSize( const std::string& filename )
{
  std::ifstream is( filename.c_str(), std::ios::in | std::ios::binary );
  if( !is.is_open() )
  {
    fprintf(stderr, "Error: cannot open file %s to determine file size\n", filename.c_str() );
    return -1;
  }
  is.seekg (0, std::ios::end);
  size_t filesize = is.tellg();
  if( is.fail() )
  {
    fprintf(stderr, "Error: failed to determine file size for file %s\n", filename.c_str() );
    return -1;
  }
  is.close();
  return filesize;
}

int packPYUV( std::istream& inStream, std::ostream& outStream, const unsigned int& samples)
{
  size_t inputSize = samples<<1;
  PayloadPtr cPayloadPtrSrc( new(std::nothrow) PayloadPtr::element_type[inputSize] );
  if( !cPayloadPtrSrc )
  {
    fprintf(stderr, "Error: memory allocation failed for input payload buffer\n");
    return -1;
  }
  size_t outputSize = samples/4*5;
  PayloadPtr payloadPtrDst( new(std::nothrow) PayloadPtr::element_type[outputSize] );
  if( !payloadPtrDst )  {
    fprintf(stderr, "Error: memory allocation failed for output payload buffer\n");
    return -1;
  }

  while ( readPayload( inStream, cPayloadPtrSrc, inputSize ) > 0 )
  {
    convertToPacked( cPayloadPtrSrc, payloadPtrDst, samples );
    outStream.write( reinterpret_cast< char* >( payloadPtrDst.get() ), outputSize );
    if( outStream.fail() )
    {
      fprintf(stderr, "Error: failed to write output payload\n");
      return -1;
    }
  }

  return 0;
}

int unpackPYUV( std::istream& inStream, std::ostream& outStream, const unsigned int& samples)
{
  size_t inputSize = samples/4*5;
  PayloadPtr cPayloadPtrSrc( new(std::nothrow) PayloadPtr::element_type[inputSize] );
  if( !cPayloadPtrSrc )
  {
    fprintf(stderr, "Error: memory allocation failed for input payload buffer\n");
    return -1;
  }
  size_t outputSize = samples<<1;
  PayloadPtr payloadPtrDst( new(std::nothrow) PayloadPtr::element_type[outputSize] );
  if( !payloadPtrDst )  {
    fprintf(stderr, "Error: memory allocation failed for output payload buffer\n");
    return -1;
  }
  while ( readPayload( inStream, cPayloadPtrSrc, inputSize ) > 0 )
  {
    convertToUnpacked( cPayloadPtrSrc, payloadPtrDst, samples );
    outStream.write( (char*)( payloadPtrDst.get() ), outputSize );
    if( outStream.fail() )
    {
      fprintf(stderr, "Error: failed to write output payload\n");
      return -1;
    }
  }

  return 0;
}

int main(int argc, char * argv[])
{
  ProgramOptionsLite::Options opts;
  std::string inputFilename;
  std::string outputFilename;
  bool do_help = false;
  bool unpack = false;
  int samplesToRead = 0;

  opts.addOptions()
  ("help",              do_help,            false,           "show help text")
  ("InputFile,i",       inputFilename,      std::string(""), "YUV/PYUV input file name")
  ("OutputFile,o",      outputFilename,     std::string(""), "YUV/PYUV output file name\n")
  ("unpack,u",          unpack,             false,           "force unpack PYUV to YUV (default: false)\n")
  ("samples",           samplesToRead,      0,               "number of samples to read at once (default: auto)\n")
  ;
  ProgramOptionsLite::SilentReporter err;
  ProgramOptionsLite::scanArgv(opts, argc, (const char**) argv, err);

  if (argc == 1 || do_help)
  {
    ProgramOptionsLite::doHelp(std::cout, opts);

    printf("Converts a yuv420 10bit file to packed yuv format and vice versa. The mode is automatically detected from the input file extension.\n");
    printf("input can be read from a pipe and output can be written to a pipe by using '-' as the filename.\n");
    printf("Unpacking can be forced with the '--unpack' option (e.g. when the packed yuv input is piped from an other application).\n");
    printf("Examples:\n");
    printf(" converts a yuv420 10bit file to packed yuv format\n");
    printf("  %s -i file.yuv -o file.pyuv\n", argv[0]);
    printf(" converts a packed yuv420 file to yuv420 10bit format \n");
    printf("  %s -i file.pyuv -o file.yuv\n", argv[0]);
    printf(" calculate md5sum of an unpacked yuv420 10bit file \n");
    printf("  %s -i file.pyuv -o - | md5sum -\n", argv[0]);
    return -1;
  }

  if ( inputFilename.empty() )
  {
    fprintf(stderr, "Error: no input file specified\n");
    return -1;
  }
  if( outputFilename.empty() )
  {
    fprintf(stderr, "Error: no output file specified\n");
    return -1;
  }
  if ( inputFilename == outputFilename && inputFilename != "-" )
  {
    fprintf(stderr, "Error: input and output filename must not be identical\n");
    return -1;
  }

  auto dot = inputFilename.find_last_of(".");
  if (dot != std::string::npos && inputFilename.substr(dot + 1) == "pyuv")
  {
    unpack = true;
  }

  auto dotOut = outputFilename.find_last_of(".");
  if (dotOut != std::string::npos && outputFilename.substr(dotOut + 1) == "pyuv" && unpack)
  {
    fprintf(stderr, "Error: input and output file extensions are inconsistent as both have the pyuv extension\n");
    return -1;
  }

  if( inputFilename == "-" )
  {
    if( samplesToRead <= 0 )  samplesToRead = 256;
  }
  else
  {
    if( samplesToRead <= 0 )
    {
      size_t filesize = getFileSize(inputFilename);
      if( filesize == (size_t)-1 )
      {
        return -1;
      }
      if( unpack )
      {
        for( int bs : {256, 128, 64, 32, 16} )
        {
          if( filesize % (bs / 4 * 5) == 0 ) { samplesToRead = bs; break; }
        }
      }
      else
      {
        for( int bs : {256, 128, 64, 32, 16} )
        {
          if( filesize % (bs * 2) == 0 ) { samplesToRead = bs; break; }
        }
      }
      if( samplesToRead <= 0 )
      {
        fprintf(stderr, "Error: cannot determine suitable chunksize\n");
        return -1;
      }
    }
  }

  if( samplesToRead % 4 != 0 )
  {
    fprintf(stderr, "Error: number of samples to read at once must be a multiple of 4\n");
    return -1;
  }

  std::fstream finHandle;
  std::fstream foutHandle;
  if( inputFilename == "-" )
  {
#if defined (_WIN32) || defined (WIN32) || defined (_WIN64) || defined (WIN64)
    if( _setmode( _fileno( stdin ), _O_BINARY ) == -1 )
    {
      return -1;
    }
#endif
  }
  else
  {
    finHandle.open( inputFilename, std::ios::binary | std::ios::in );
    if( finHandle.fail() )
    {
      fprintf(stderr, "Cannot open file %s\n", inputFilename.c_str() );
      return -1;
    }
  }
  if( outputFilename == "-" )
  {
#if defined (_WIN32) || defined (WIN32) || defined (_WIN64) || defined (WIN64)
    if( _setmode( _fileno( stdout ), _O_BINARY ) == -1 )
    {
      return -1;
    }
#endif
  }
  else
  {
    foutHandle.open( outputFilename, std::ios::binary | std::ios::out );
    if( foutHandle.fail() )
    {
      fprintf(stderr, "Cannot open file %s\n", outputFilename.c_str() );
      return -1;
    }
  }

  std::istream& inStream  = ( inputFilename == "-" ) ? std::cin : finHandle;
  std::ostream& outStream = ( outputFilename == "-" ) ? std::cout : foutHandle;

  int ret = 0;
  if ( unpack )
  {
    fprintf(stderr, "Unpack PYUV file %s\n", inputFilename.c_str());
    ret = unpackPYUV(inStream, outStream, samplesToRead);
  }
  else
  {
    fprintf(stderr, "Pack YUV file %s\n", inputFilename.c_str());
    ret = packPYUV(inStream, outStream, samplesToRead);
  }

  if( inputFilename != "-" && finHandle.is_open())
  {
    finHandle.close();
  }

  if( outputFilename != "-"  && foutHandle.is_open())
  {
    foutHandle.close();
  }

  if( ret != 0 )
  {
    fprintf(stderr, "Error: conversion failed with error code %d\n", ret);
    return -1;
  }

  return ret;
}

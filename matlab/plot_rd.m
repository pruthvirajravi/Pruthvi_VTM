% plot_rd.m  —  Publication-ready RD curves from results.csv
T = readtable('results.csv');
T = T(~isnan(T.bitrate_kbps) & ~isnan(T.yuv_psnr), :);
seqs = unique(T.sequence);
fig = figure('Units','inches','Position',[1 1 6.0 4.2]);
ax = axes(fig); hold(ax,'on'); grid(ax,'on'); box(ax,'on');
mk = {'-o','-s','-^','-d','-v','-p','->','-<'};
for k = 1:numel(seqs)
    S = sortrows(T(strcmp(T.sequence, seqs{k}), :), 'bitrate_kbps');
    plot(ax, S.bitrate_kbps, S.yuv_psnr, mk{mod(k-1,numel(mk))+1}, ...
        'LineWidth',1.4, 'MarkerSize',6, 'DisplayName', seqs{k});
end
xlabel(ax,'Bitrate (kbps)','FontName','Times New Roman','FontSize',11);
ylabel(ax,'YUV-PSNR (dB)','FontName','Times New Roman','FontSize',11);
set(ax,'FontName','Times New Roman','FontSize',10);
legend(ax,'Location','southeast','Interpreter','none','FontSize',8);
exportgraphics(fig,'rd_curves.pdf','ContentType','vector');
exportgraphics(fig,'rd_curves.png','Resolution',600);
disp('Saved rd_curves.pdf and rd_curves.png');

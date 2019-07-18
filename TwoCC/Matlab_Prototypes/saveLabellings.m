function saveLabellings (lbls_list, filename)

    fp = fopen (filename, 'w');

    for i=1:length(lbls_list)
    
        lbls = lbls_list{i};
        
        n = length (lbls);

        fprintf (fp, '[');
        for i=1:n
            fprintf (fp, '%d', lbls(i));
            if (i ~= n)
                fprintf (fp, ', ');
            end
        end
        fprintf (fp, ']');
    end
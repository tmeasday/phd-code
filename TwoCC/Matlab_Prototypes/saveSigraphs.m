function saveSigraphs (Advices, filename)

    fp = fopen (filename, 'w');


    for i=1:length(Advices)

        Sigraph = Advices{i};
        n = size (Sigraph, 1);

        fprintf (fp, '[');
        for i=1:n

            fprintf (fp, '[');
            for j=i+1:n
                fprintf (fp, '%d', Sigraph(i,j));
                if (j ~= n)
                    fprintf (fp, ', ');
                end
            end

            fprintf (fp, ']');
            if (i ~= n)
                fprintf (fp, ', ');
            end
        end
        fprintf (fp, ']\n');
    end
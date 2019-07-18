function saveSigraph(Sigraph, filename)

    n = size (Sigraph, 1);
   
    fp = fopen (filename, 'w');
    
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
    fprintf (fp, ']');
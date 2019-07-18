function saveLabelling (lbls, filename)

    n = length (lbls);
   
    fp = fopen (filename, 'w');
    
    fprintf (fp, '[');
    for i=1:n
        fprintf (fp, '%d', lbls(i));
        if (i ~= n)
            fprintf (fp, ', ');
        end
    end
    fprintf (fp, ']');
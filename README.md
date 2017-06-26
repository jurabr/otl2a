The otl2a is a very naive TVO (The Vim Outliner) to HTML/LaTeX format converter.

Installation
============

It is written in the plain old ANSI C89 language.

1. do: CC -o otl2a otl2a.c
2. copy otl2a any directory listed in the  PATH variable (say
   /usr/local/bin) and otl2a.1 to one of MANPATH directories
   (e.g. /usr/local/man/man1/ or so)


Output formats
==============

We are currently very limited. You may get:

1. HTML (very simple HTML - no styles, no nothing)
2. LaTeX (article) 
3. LaTeX (book) 
4. LaTeX (slides - but my modification with predefined "\sltit" and
   "\slsec" commands instead of \section, \subsection etc)
5. EMACS outliner format (untested as I am not an EMACS user)
6. LaTeX Beamer format (presentations)


Usage
=====

otl2a [options] <input >output

See manual page for options.

The most inportant options:

-t=format
         Output  format.  format  can be on of: html, latex, lslides,
         lbeamer,  lbook  emacs.  The long-style options   (--html  or
         --latex  or  --latex-slides, --latex-beamer, --latex-book or
         --emacs instead  of -t=format) also may work.

 -e or --expand-links
         Links  (e.g. [link], [[anchor]] [file.ext]) will be expanded
         (replaced  with  format-depedent informa- tion).   If
         links.ext  is an  image (extension is identical to one of
         png, jpg, gif, jpeg, tif, tiff, eps, svg), the proper image
         tag will be used (<img src=..> for the HTML).

-l or --expand-hlinks
         The http://...,  https://...  and  ftp://...  will   be
         expanded/replaced with proper hyperlink tags.

-u or --itemize-only or --no-enumerate
         All itemized things (lines that starts with "*"  or "-")
         will  be  exported as non-numbered items. The default
         behaviour is to export  "-"s  as  numbered items (<ol> in
         HTML).


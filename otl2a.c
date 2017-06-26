/*
   File name: otl2a.c
   Date:      2004/10/15 19:04
   Author:    Jiri Brozovsky

   Copyright (C) 2004 Jiri Brozovsky

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   in a file called COPYING along with this program; if not, write to
   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA
   02139, USA.

   Converts TVO (The Vim Outliner) files to HTML or LaTeX files

   Usage: otl2a options <input.file >output.file

   $Id: otl2a.c,v 1.7 2005/02/13 23:25:44 jirka Exp $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGHT 2049

FILE *fr = NULL ;
FILE *fw = NULL ;
char  line[LINE_LENGHT];
int   body_only     = 0 ; /* 1=do not write header and footer of tile */
int   comment_texts = 0 ; /* texts in paragraphs will be commented    */
int   no_texts      = 0 ; /* texts in paragraphs will not be written  */
int   item_mode     = 0 ; /* 0=as is, 1=itemize, no enumerate         */
int   expand_links  = 0 ; /* if [eee] will be expanded ==1 or no ==0  */
int   expand_hlinks = 0 ; /* if http:///eee will be expanded ==1 or no ==0  */
int   slidenumber   = 0 ; /* if == 1 it is a first slide, 0== other   */
int   upc           = 0 ; /* 1 == include \usapackage{czech} in LaTeX headers */

int   outtype = 0 ; /* 0=ascii 1=html 2=latex 3=latex slides, 4=emacs, 5=latex book */


/* Stuff definitions - start ------------------------ */

/** file header */
void file_start(void)
{
	if (body_only == 1) {return;}

	switch(outtype)
	{
		case 0 : break ;
		case 1 : fprintf(fw,"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n<html>");
						 break ;
		case 2 : fprintf(fw,"\\documentclass{article}\n\\usepackage{graphicx}\n");
             if (upc == 1) {fprintf(fw,"\\usepackage{czech}\n");}
             fprintf(fw,"\n\\begin{document}");
						 break ;
		case 3 : fprintf(fw,"\\documentclass[landscape]{slides}\n\\usepackage{graphicx}\n\\usepackage{color}\n\\usepackage{palatino}\n\\usepackage{amsmath}\n");
						 fprintf(fw,"\n\\voffset = -26 mm\n\\textheight = 184 mm");
						 fprintf(fw,"\n\\newcommand{\\sltit}[1]{\n");
						 fprintf(fw," \\textbf{\\color{blue}\\huge #1}}\n");
						 fprintf(fw,"\\newcommand{\\slsect}[1]{\n");
						 fprintf(fw," \\textbf{\\color{red}\\large #1}}\n");
             if (upc == 1) {fprintf(fw,"\\usepackage{czech}\n");}
						 fprintf(fw,"\n\n\n\\begin{document}\n");
						 fprintf(fw," \\pagestyle{plain}\n");
						 break ;
		case 4 : break ;
		case 5 : fprintf(fw,"\\documentclass{book}\n\\usepackage{graphicx}\n");
             if (upc == 1) {fprintf(fw,"\\usepackage{czech}\n");}
             fprintf(fw,"\n\\begin{document}");
						 break ;
		case 6 : fprintf(fw,"\\documentclass{beamer}\n\\usepackage{times}\n\\usepackage{fontenc}\n");
             if (upc == 1) { fprintf(fw,"\\usepackage[czech]{babel}\n"); fprintf(fw,"\\usepackage[latin2]{inputenc}\n"); }
             fprintf(fw,"\n\\mode<presentation>\n{\n \\usetheme{default}\n \\usecolortheme{default}\n \\setbeamercovered{transparent}\n}\n");
             fprintf(fw,"\n\\begin{document}");
             fprintf(fw,"\n \\title{}\n \\author{}\n \\institute{}\n \\date{}");
             fprintf(fw,"\n%% TITLE SLIDE\n \\begin{frame}\n \\titlepage \n\\end{frame}\n");
						 break ;

	}
	fputc('\n',fw);
}

/** file footer */
void file_end(void)
{
	if (body_only != 1)
	{
		switch(outtype)
		{
			case 0 : break ;
			case 1 : fprintf(fw,"\n</html>\n");
						 	 break ;
			case 2 : 
			case 3 : 
			case 5 : 
			case 6 : 
							 fprintf(fw,"\n\\end{document}\n");
						 	 break ;
			case 4 : break ;
		}
	}
}

/** start of slide (if applicable)
 * @param number - number of slide
 */
void slide_start(int number)
{
	switch(outtype)
	{
		case 0 : break ;
		case 1 : break ;
		case 2 : break ;
		case 3 : fprintf(fw,"\n%% SLIDE %2i\n\\begin{slide}\n",number);
						 break ;
		case 4 : break ;
		case 5 : break ;
		case 6 : fprintf(fw,"\n%% SLIDE %2i\n\\begin{frame}\n",number);
						 break ;
	}
}

/** end of slide (if applicable)
 */
void slide_end(void)
{
	switch(outtype)
	{
		case 0 : break ;
		case 1 : break ;
		case 2 : break ;
		case 3 : fprintf(fw,"\\end{slide}\n"); break ;
		case 4 : break ;
		case 5 : break ;
		case 6 : fprintf(fw,"\\end{frame}\n"); break ;
	}
}


/** text heading (<h1> and so) start
 * @param level level of heading
 */
void heading_start(int level)
{
	if (level == 1) /* slides */
	{
    /* close previous slide */
		if (slidenumber > 0) { slide_end() ; }

    slidenumber++ ;

		slide_start(slidenumber) ; /* open new slide */
	}

	switch(outtype)
	{
		case 0 : break ;
		case 1 :
						 fprintf(fw,"\n<h%i>",level);
						 break ;
		case 2 :
						 switch (level)
						 {
							 case 1: fprintf(fw,"\n\\section{"); break;
							 case 2: fprintf(fw,"\n\\subsection{"); break;
							 case 3: fprintf(fw,"\n\\subsubsection{"); break;
							 case 4: fprintf(fw,"\n\\paragraph{"); break;
							 case 5: fprintf(fw,"\n\\subparagraph{"); break;
							 case 6: fprintf(fw,"\n\\subsubparagraph{"); break;
							 case 7: 
							 case 8: 
							 case 9: 
							 case 10: 
											 fprintf(fw,"\n\\subsubsubparagraph{"); break;
						 }
						 break ;

		case 3 :
						 switch (level)
						 {
							 case 1: fprintf(fw,"\n\\sltit{"); break;
							 case 2: 
							 case 3: 
							 case 4: 
							 case 5: 
							 case 6: 
							 case 7: 
							 case 8: 
							 case 9: 
							 case 10: fprintf(fw,"\n\\slsect{"); break;
						 }
						 break ;
		case 4 :
						 switch (level)
						 {
							 case 1: fprintf(fw,"\n* "); break;
							 case 2: fprintf(fw,"\n** "); break;
							 case 3: fprintf(fw,"\n*** "); break;
							 case 4: fprintf(fw,"\n**** "); break;
							 case 5: fprintf(fw,"\n***** "); break;
							 case 6: fprintf(fw,"\n****** "); break;
							 case 7: fprintf(fw,"\n******* "); break;
							 case 8: fprintf(fw,"\n******** "); break;
							 case 9: fprintf(fw,"\n********* "); break;
							 case 10: fprintf(fw,"\n********* "); break;
						 }
						 break ;

		case 5 :
						 switch (level)
						 {
							 case 1: fprintf(fw,"\n\\chapter{"); break;
							 case 2: fprintf(fw,"\n\\section{"); break;
							 case 3: fprintf(fw,"\n\\subsection{"); break;
							 case 4: fprintf(fw,"\n\\subsubsection{"); break;
							 case 5: fprintf(fw,"\n\\paragraph{"); break;
							 case 6: fprintf(fw,"\n\\subparagraph{"); break;
							 case 7: fprintf(fw,"\n\\subsubparagraph{"); break;
							 case 8: 
							 case 9: 
							 case 10: 
											 fprintf(fw,"\n\\subsubsubparagraph{"); break;
						 }
						 break ;
		case 6 :
						 switch (level)
						 {
							 case 1: fprintf(fw,"\n\\frametitle{"); break;
							 case 2: 
							 case 3: 
							 case 4: 
							 case 5: 
							 case 6: 
							 case 7: 
							 case 8: 
							 case 9: 
							 case 10: fprintf(fw,"\n\\textbf{"); break;
						 }
						 break ;



	}
}

/** text heading end
 * @param level level of heading
 */
void heading_end(int level)
{
	switch(outtype)
	{
		case 0 : break ;
		case 1 : fprintf(fw,"</h%i>",level);
						 break ;
		case 2 : 
		case 3 : 
		case 5 : 
		case 6 : 
             fprintf(fw,"}");
						 break ;
		case 4 : break ;
	}
	fprintf(fw,"\n");
}


/** start of paragraph */
void paragraph_begin(void)
{
	if ((no_texts != 1)&&(comment_texts != 1))
	{
		switch(outtype)
		{
			case 0 : break ;
			case 1 : fprintf(fw,"<p>\n");
						 	 break ;
			case 2 :
			case 3 : 
			case 5 : 
			case 6 : 
               fprintf(fw,"\n"); break ;
			case 4 : break ;
		}
		/*fprintf(fw,"\n");*/
	}
}

/** end of paragraph */
void paragraph_end(void)
{
	if ((no_texts != 1)&&(comment_texts != 1))
	{
		switch(outtype)
		{
			case 0 : break ;
			case 1 : fprintf(fw,"</p>\n\n");
						 	 break ;
			case 2 : 
			case 3 :
			case 5 :
			case 6 :
               fprintf(fw,"\n\n"); break ;
			case 4 : break ;
		}
	}
}

/** start of comment */
void comment_begin(void)
{
	if (comment_texts == 1)
	{
		switch(outtype)
		{
			case 0 : break ;
			case 1 : fprintf(fw,"<!-- ");
						 	 break ;
			case 2 : 
			case 3 : 
			case 5 : 
			case 6 : 
               fprintf(fw,"%% "); break ;
			case 4 : break ;
		}
	}
}

/** end of comment */
void comment_end(void)
{
	if (comment_texts == 1)
	{
		switch(outtype)
		{
			case 0 : break ;
			case 1 : fprintf(fw," -->");
						 	 break ;
			case 2 : break ;
			case 3 : break ;
			case 4 : break ;
			case 5 : break ;
			case 6 : break ;
		}
	}
}


/* start of "itemize" */
void itemize_begin(int itype)
{
	int type ;

	if (item_mode == 0) { type = itype ; }
	else                { type = 2 ; }

	switch(outtype)
	{
		case 0 : break ;
		case 1 :
						 if (type == 1) { fprintf(fw,"<ol>\n"); }
						 else           { fprintf(fw,"<ul>\n"); }
						 break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
						 if (type == 1) { fprintf(fw,"\\begin{enumerate}\n"); }
						 else           { fprintf(fw,"\\begin{itemize}\n"); }
						 break ;
		case 4 : break ;
	}
}

/* end of "itemize" */
void itemize_end(int itype)
{
	int type ;

	if (item_mode == 0) { type = itype ; }
	else                { type = 2 ; }

	switch(outtype)
	{
		case 0 : break ;
		case 1 :
						 if (type == 1) { fprintf(fw,"</ol>\n"); }
						 else           { fprintf(fw,"</ul>\n"); }
						 break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
						 if (type == 1) { fprintf(fw,"\\end{enumerate}\n"); }
						 else           { fprintf(fw,"\\end{itemize}\n"); }
						 break ;
		case 4 : break ;
	}
}

void item_begin(int type)
{
	switch(outtype)
	{
		case 0 : fprintf(fw,"* "); break ;
		case 1 : fprintf(fw,"<li> "); break ;
		case 2 : 
		case 3 : 
		case 5 : 
		case 6 : 
             fprintf(fw,"\\item "); break ;
		case 4 : fprintf(fw,"- "); break ;
	}
}

void item_end(void)
{
	switch(outtype)
	{
		case 0 : break ;
		case 1 : fprintf(fw,"</li>\n"); break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"\n"); break ;
		case 4 : break ;
	}
}

void write_anchor(char *s, int from, int to)
{
	int i ;
	switch(outtype)
	{
		case 0 : fprintf(fw,"[["); break ;
		case 1 : fprintf(fw,"<a name=\""); break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"{label=\""); break ;
		case 4 : fprintf(fw,"[["); break ;
	}
	
	for (i=from; i<=to; i++) { fputc(s[i],fw); }

	switch(outtype)
	{
		case 0 : fprintf(fw,"]] "); break ;
		case 1 : fprintf(fw,"\">"); 
						 for (i=from; i<=to; i++) { fputc(s[i],fw); }
						 fprintf(fw,"</a> "); 
						 break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"\"} "); break ;
		case 4 : fprintf(fw,"]] "); break ;
	}

	/*fprintf(fw,"\n");*/
}

void write_image(char *s, int from, int to)
{
	int i ;
	switch(outtype)
	{
		case 0 : fprintf(fw,"[["); break ;
		case 1 : fprintf(fw,"<img src=\""); break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"\\includegraphics{"); break ;
		case 4 : fprintf(fw,"[["); break ;
	}
	
	for (i=from; i<=to; i++) { fputc(s[i],fw); }

	switch(outtype)
	{
		case 0 : fprintf(fw,"]] "); break ;
		case 1 : fprintf(fw,"\" alt=\""); 
						 for (i=from; i<=to; i++) { fputc(s[i],fw); }
						 fprintf(fw,"\" /> "); 
						 break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"} "); break ;
		case 4 : fprintf(fw,"]] "); break ;
	}

	/*fprintf(fw,"\n");*/
}

void write_link(char *s, int from, int to)
{
	int i ;
	switch(outtype)
	{
		case 0 : fprintf(fw,"["); break ;
		case 1 : fprintf(fw,"<a href=\"#"); break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"\\ref{"); break ;
		case 4 : fprintf(fw,"["); break ;
	}
	
	for (i=from; i<=to; i++) { fputc(s[i],fw); }

	switch(outtype)
	{
		case 0 : fprintf(fw,"] "); break ;
		case 1 : fprintf(fw,"\"> "); 
						 for (i=from; i<=to; i++) { fputc(s[i],fw); }
						 fprintf(fw,"</a> "); 
						 break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"} "); break ;
		case 4 : fprintf(fw,"] "); break ;
	}
}

void write_file_link(char *s, int from, int to)
{
	int i ;
	switch(outtype)
	{
		case 0 : fprintf(fw,"["); break ;
		case 1 : fprintf(fw,"<a href=\""); break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"\\input{"); break ;
		case 4 : fprintf(fw,"["); break ;
	}
	
	for (i=from; i<=to; i++) { fputc(s[i],fw); }

	switch(outtype)
	{
		case 0 : fprintf(fw,"] "); break ;
		case 1 : fprintf(fw,"\"> "); 
						 for (i=from; i<=to; i++) { fputc(s[i],fw); }
						 fprintf(fw,"</a> "); 
						 break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"} "); break ;
		case 4 : fprintf(fw,"} "); break ;
	}

	/*fprintf(fw,"\n");*/
}

void write_file_hlink(char *s, int from, int to)
{
	int i ;
	switch(outtype)
	{
		case 0 : fprintf(fw,"["); break ;
		case 1 : fprintf(fw,"<a href=\""); break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"{\\tt "); break ;
		case 4 : fprintf(fw,"["); break ;
	}
	
	for (i=from; i<=to; i++) { fputc(s[i],fw); }

	switch(outtype)
	{
		case 0 : fprintf(fw,"] "); break ;
		case 1 : fprintf(fw,"\"> "); 
						 for (i=from; i<=to; i++) { fputc(s[i],fw); }
						 fprintf(fw,"</a> "); 
						 break ;
		case 2 :
		case 3 :
		case 5 :
		case 6 :
             fprintf(fw,"} "); break ;
		case 4 : fprintf(fw,"] "); break ;
	}

	/*fprintf(fw," ");*/
}

/* Stuff definitions - end -------------------------- */

/** tests what is inside: 0=nothing 1=image, 2=file, 3=name/link */
int test_stuff_type(char *s, int from, int to)
{
	int i, j ;
	int is_file = 0 ;
	int dot_pos = -1 ;
	char big[]  ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char small[]="abcdefghijklmnopqrstuvwxyz";
	char ts[LINE_LENGHT];

	if ((to-from) < 1) {return(0);}  /* empty brackets?              */
	if (s[from] == ':') {return(0);} /* Vim command, we'll ignore it */

	/* file should have an extension (something.blah): */
	for (i=from; i<=to; i++)
	{
		if ((s[i] == '.')&&(i > from)&&(i<to))
		{
			is_file = 1 ; /* assume it's a file */
			dot_pos = i+1 ;
			break ;
		}
	}


	if (is_file == 0) {return(3);} /* probably a link */

  if (dot_pos == -1) {return(0);} /* unidentified stuff - leave untouched */

	/* now we have to test if it's an image */
	for (i=dot_pos; i<=to; i++) 
	{ 
		ts[i-dot_pos] = s[i] ; 

		for (j=0; j<26; j++)
		{
			if (ts[i-dot_pos] == big[j]) 
			{
				ts[i-dot_pos] = small[j] ;
				break ;
			}
		}
	}

	ts[(to-dot_pos)+1] = '\0' ;

	if (strcmp(ts,"jpg") == 0) {return(1);}
	if (strcmp(ts,"jpeg") == 0) {return(1);}
	if (strcmp(ts,"png") == 0) {return(1);}
	if (strcmp(ts,"gif") == 0) {return(1);}
	if (strcmp(ts,"tif") == 0) {return(1);}
	if (strcmp(ts,"tiff") == 0) {return(1);}
	if (strcmp(ts,"eps") == 0) {return(1);}
	if (strcmp(ts,"svg") == 0) {return(1);}

	/* it isn't an image if we are here */
	return(2);
}

int check_hlink(char *s, int from, int to)
{
	int i ;
	int is_hlink = 1 ;
	char a1[] = "ftp://" ;
	char a2[] = "http://" ;
	char a3[] = "https://" ;

	if ((s[from] != 'h') && (s[from] != 'f')) {return(from);}
	if ((s[from] == 'f') && ((to-from+1) < strlen(a1))) {return(from);}
	if ((s[from] == 'h') && ((to-from+1) < strlen(a2))) {return(from);}

	if (s[from] == 'f')
	{
		for (i=0; i< strlen(a1); i++)
		{
			if (a1[i] != s[from+i]) {return(from);}
		}
	}
	else
	{
		is_hlink = 1 ;
		for (i=0; i< strlen(a2); i++)
		{
			if (a2[i] != s[from+i]) {is_hlink = 0; break; }
		}

		if (is_hlink == 0)
		{
			if ((to-from+1) < strlen(a3))
			{
				return(from);
			}
			else
			{
				for (i=0; i< strlen(a3); i++)
				{
					if (a3[i] != s[from+i]) {return(from); }
				}
			}
		}
	}

	for (i=from; i<=to; i++)
	{
		if ((s[i]==' ')||(s[i]=='\t')||(s[i]=='\0')||(s[i]=='\n')||(s[i]==',')||(s[i]==')')||(s[i]=='\\')||(s[i]=='}'))
		{
			return(i-1);
		}
	}

	return(to);
}

/** Writes text line
 * @param string with text
 * @param from starting position of text to be written
 * @param to ending position of text to be written
 */
int write_line(char *s, int from, int to)
{
	int i,j,k, pos ;
	int stuff_found = 0 ;
	
	if (expand_links == 1)
	{
		for (i=from; i<=to;i++) 
		{ 
			if (s[i] !='[')
			{
				if (expand_hlinks == 1)
				{
					if ((pos=check_hlink(s, i, to)) == i)
					{
						fputc(s[i],fw); /* no hyperlink */
					}
					else
					{
						/* hyperlink - write it */
						write_file_hlink(s, i, pos);
						i = pos ; 
						continue ;
					}
				}
				else
				{
					fputc(s[i],fw); /* no nothing */
				}
			}
			else
			{
				if (i == to) { fputc(s[i],fw); return(0); } /* mess at the end of line*/ 

				if (s[i+1] == '[') /* it's most probably an anchor */
				{
					if (i+2 < to)
					{
						for (j=(i+2); i<to; j++)
						{
							if (s[j] == ']')
							{
								if (s[j+1] == ']')
								{
									/* it is an anchor */
									write_anchor(s, i+2, j-1) ;
									i = j+2 ;
									continue ;
								}
								else
								{
									/* it's a mess! */
									for (k=i; k<=(j); k++)
									{
										fputc(s[k],fw); 
									}
									i = j ;
									continue ;
								}
							}
						}
					}
					else
					{
						/* probably something incomplete */
						for (j=i; j<=to; j++)
						{
							fputc(s[j],fw); 
						}
						return(0);
					}
				}
				else
				{
					/* not an anchor - maybe file or image */
					stuff_found = 0 ;
					for (j=i+1; j<=to; j++)
					{
						if (s[j] == ']')
						{
							stuff_found = 1 ;

							switch (test_stuff_type(s, i+1, j-1))
							{
								case 0: stuff_found = 0; break ;
								case 1: write_image(s, i+1,j-1); break ;
								case 2: write_file_link(s, i+1,j-1); break ;
								case 3: write_link(s, i+1,j-1); break ;
							}

							i = j ;
							break ; ;
						}
					}
					if (stuff_found == 0)
					{
						fputc(s[i],fw) ; /* something is strange - ignore it */
						continue ;
					}
				}
			}
		}
	}
	else
	{
		/* simple and working code: */
		for (i=from; i<=to;i++) { fputc(s[i],fw); }
	}
	return(0) ;
}


/** Tests the type of line
 * @param s line
 * @param item 0 = it's not a item 1=itemize 2=enumerate
 * @param itemcont 1 = continuation of item from previous line
 * @param from starting position for reading of data
 * @param to ending position for reading of data
 * @return  0 == text, 1-10 == headers, -1 empty line 
 */
int test_line_type(char *s, int *item, int *itemcont, int *from, int *to)
{
	int i ;
	int count ;
	
	*item     = 0 ;
	*itemcont = 0 ;
	*from     = 0 ;
	*to       = strlen(s) - 2 ; ;

	if (*to < 0) {return(-1);}

	count = 1 ;

	for (i=0; i<strlen(s); i++)
	{
		*from = 0 ;

		if (s[i] == '\t') /* TAB character is here! */
		{
      if (s[i+1] == '\n')
      {
        return (-1); /* empty line - ignore */
      }
      else
      {
			  count++;
      }
		}
		else
		{
			if ((s[i] != '|')&&(s[i] != '*')&&(s[i] != '-')) /* heading */
			{	
				*from = i ;

				/* if count == i+1 that it is continuation of itemize and not a heading! */
				if (s[i] == ' ') { *itemcont = 1 ; }
				return(count) ;
			}
			else /* paragraph */
			{
				if ((i+1) >= strlen(s))
				{
					return(-1); /* empty line - ignore it */
				}
				else /* there is something (maybe) interesting */
				{
					*from = i+1 ;

					if (s[i] == '|')
					{
						return(0) ; /* paragraph */
					}

					if (s[i] == '-') /* enumerate */
					{
						*item = 1 ;
						return(count) ;
					}

					if (s[i] == '*') /* itemize   */
					{
						*item = 2 ;
						return(count) ;
					} 
				}
			}
		}
	}

	return(-1) ; /* if everything failed we can assume it's an empty line */
}

void clean_line(void)
{
	int i ;

	for (i=0; i<LINE_LENGHT; i++) {line[i] = '\0';}
}


int read_cmd_line(int argc, char *argv[])
{
	int i ;

	for (i=0; i<argc; i++)
	{
		if ((strcmp("-h",argv[i])==0)||(strcmp("-H",argv[i])==0)||(strcmp("--help",argv[i])==0))
		{
			printf("RTFM!\n"); 
			exit(-1);
		}

		if ((strcmp("-b",argv[i])==0)||(strcmp("-B",argv[i])==0)||(strcmp("--body-only",argv[i])==0))
		   { body_only = 1 ; }

		if ((strcmp("-c",argv[i])==0)||(strcmp("-C",argv[i])==0)||(strcmp("--comment-texts",argv[i])==0))
		   { comment_texts = 1 ; }

		if ((strcmp("-n",argv[i])==0)||(strcmp("-N",argv[i])==0)||(strcmp("--no-texts",argv[i])==0))
		   { no_texts = 1 ; }

		if ((strcmp("-u",argv[i])==0)||(strcmp("--itemize-only",argv[i])==0)||(strcmp("--no-enumerate",argv[i])==0))
		   { item_mode = 1 ; }

		if ((strcmp("-e",argv[i])==0)||(strcmp("-E",argv[i])==0)||(strcmp("--expand-links",argv[i])==0))
		   { expand_links = 1 ; }

		if ((strcmp("-l",argv[i])==0)||(strcmp("-L",argv[i])==0)||(strcmp("--expand-hlinks",argv[i])==0))
		   { expand_hlinks = 1 ; }

		if ((strcmp("-upc",argv[i])==0)||(strcmp("--upc",argv[i])==0)||(strcmp("--usepackage-czech",argv[i])==0))
		   { upc = 1 ; }

    /* output formats: */
		if ((strcmp("-t=html",argv[i])==0)||(strcmp("--html",argv[i])==0))
		   { outtype = 1 ; }

		if ((strcmp("-t=latex",argv[i])==0)||(strcmp("-t=LaTeX",argv[i])==0)||(strcmp("--latex",argv[i])==0)||(strcmp("--LaTeX",argv[i])==0))
		   { outtype = 2 ; }

		if ((strcmp("-t=lslides",argv[i])==0)||(strcmp("--lslides",argv[i])==0)||(strcmp("--latex-slides",argv[i])==0))
		   { outtype = 3 ; }

		if ((strcmp("-t=emacs",argv[i])==0)||(strcmp("--emacs",argv[i])==0)||(strcmp("--EMACS",argv[i])==0))
		   { outtype = 4 ; }

		if ((strcmp("-t=lbook",argv[i])==0)||(strcmp("--lbook",argv[i])==0)||(strcmp("--latex-book",argv[i])==0))
		   { outtype = 5 ; }

		if ((strcmp("-t=lbeamer",argv[i])==0)||(strcmp("--lbeamer",argv[i])==0)||(strcmp("--latex-beamer",argv[i])==0))
		   { outtype = 6 ; }

#if 0
		if ((strcmp("-t=mgp",argv[i])==0)||(strcmp("--mgp",argv[i])==0)||(strcmp("--magicpoint",argv[i])==0))
		   { outtype = 6 ; }
#endif
	}
	return(0);
}


/* main() function - what to say about it? */
int main(int argc, char *argv[])
{
	int type, item, itemcont, from, to ;
	int prevline = -1 ; /* -1=empty 0=text 1=heading 2=itemize/enumerate */
	int previlevel = -1 ; /* itemlevel of previous line */
	int itemlevel = 0 ; /* number of levels of "itemize" */
	int itemtype[21] ;  /* "itemize" types */
	int itemstyle[21] ;  /* "itemize" style ul/ol */
	int firstitem = 0 ;
	int i,j ;

	read_cmd_line(argc, argv);

	fr = stdin ;
	fw = stdout ;

	for (i=0; i<21; i++) 
	{ 
		itemtype[i] = '\0'; 
		itemstyle[i] = 0; 
	}

	file_start();

	while (fgets(line,LINE_LENGHT-1, fr))
	{
		type = test_line_type(line, &item, &itemcont, &from, &to) ;

		if (type == -1) 
		{ 
			fprintf(fr,"\n"); 
			clean_line(); 
			continue ; 
		}

		if ((type >=1)&&(type <= 10)&&(itemcont == 0)&&(item == 0))
		{
			/* heading */
			if (prevline == 0){paragraph_end();} 
			if (prevline == 2)
			{
				item_end(); 
				for (j=itemlevel; j>=1; j-- )
				{
				  itemize_end(itemstyle[j]);
				}
        itemlevel = -1 ;
			} 

			heading_start(type);
			write_line(line, from, to) ;
			heading_end(type);
			clean_line();
			prevline = -1 ;
			previlevel = 0 ;
			continue;
		}

		if ((itemcont != 0)&&(itemlevel > 0)) /* item continuation */
		{
			write_line(line, from, to) ;
			fprintf(fw,"\n");
			prevline = 2 ;
			continue;
		}

		if ((type == 0)&&(item == 0)) /* plain text */
		{
			if (prevline == 2){item_end(); itemize_end(itemstyle[itemlevel]); itemlevel--;} /* is the -- OK? */
			if (prevline != 0){paragraph_begin();} 

			if (no_texts != 1)
			{
				comment_begin();
				write_line(line, from, to) ;
				comment_end();
				fprintf(fw,"\n");
			}
			prevline = 0 ;
			continue;
		}
		else /* item */
		{
			if (prevline == 0){paragraph_end();} 

			if (itemlevel <= 0)
			{
				itemlevel = 1 ;
				itemtype[itemlevel] = type ;
				itemstyle[itemlevel] = item ;
				itemize_begin(item) ;
				firstitem=1;
			}

			if (itemtype[itemlevel] < type)
			{
				/*item_begin(item);
				fprintf(fw,"\n");*/

				itemlevel++ ;
				itemtype[itemlevel] = type ;
				itemstyle[itemlevel] = item ;
				itemize_begin(item) ;

				item_begin(item);
				write_line(line, from, to) ;
						firstitem = 0 ;
				/*item_end();*/
			}
			else
			{
				if (itemtype[itemlevel] == type)
				{
					if (firstitem !=1 )
					{
            if  (prevline == 2)
            {
						  item_end();
            }
            else
            {
				      itemtype[itemlevel] = type ;
				      itemstyle[itemlevel] = item ;

              itemize_begin(item) ;
            }
					}
					firstitem = 0 ;

					item_begin(item);
					write_line(line, from, to) ;
					/*item_end(item);*/
				}
				else
				{
					item_end();
				
					itemize_end(itemstyle[itemlevel]); 
					itemlevel--;

					item_end(); 

					item_begin(item);
					write_line(line, from, to) ;
						firstitem = 0 ;
					/*item_end(item);*/
				}
			}

			prevline = 2 ;
			continue;
		}
	}

  /* close still opened itemizes: */
  if ((prevline == 2) && (itemlevel > -1))
	{
		item_end(); 
		for (j=itemlevel; j>=1; j-- )
		{
			itemize_end(itemstyle[j]);
		}
    itemlevel = -1 ;
	}

	slide_end();

	file_end();

	fclose(fr);
	fclose(fw);
	return(0);
}

/* end of otl2a.c */

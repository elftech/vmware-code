#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "csv.h"

enum{ NOMEM=-2 }; /* out of memory signal*/

static char *g_line=NULL;
static char *g_sline=NULL;
static int g_maxline=0;
static char **g_field=NULL;
static int g_maxfield=0;
static int g_nfield=0;

static char g_field_sep[]=",";

/* assistance functions */
static int bEndOfLine(FILE *fin,int c)
{
	int b_end;
	b_end=(c=='\r'||c=='\n');
	if(c=='\r'){
		c=getc(fin);
		if(c!='\n'&&c!=EOF)
			ungetc(c,fin);
	}
	return b_end;
}

static void reset(void)
{
	free(g_line);
	free(g_sline);
	free(g_field);
	g_line=NULL;
	g_sline=NULL;
	g_field=NULL;
	g_maxfield=g_maxfield=g_nfield=0;
}

static char *postFieldSep(char *p)
{
	int i,j;
	for(i=j=0;p[j]!='\0';++i,++j){
		if(p[j]=='"'&&p[++j]!='"'){
			int k=strcspn(p+j,g_field_sep);
			memmove(p+i,p+j,k);
			i+=k;
			j+=k;
		}
		p[i]=p[j];
	}
	p[i]='\0';
	return p+j;
}

static int split(void)
{
	char *p,**newf;
	char *p_seperator;
	int seperator;

	g_nfield=0;
	if(g_line[0]=='\0')
		return 0;
	strcpy(g_sline,g_line);
	p=g_sline;
	do{
		if(g_nfield>=g_maxfield){
			g_maxfield*=2;
			newf=(char **)realloc(g_field,g_maxfield*sizeof(g_field[0]));
			if(newf==NULL)
				return NOMEM;
			g_field=newf;
		}
		if(*p=='"')
			p_seperator=postFieldSep(++p);
		else
			p_seperator=p+strcspn(p,g_field_sep);
		seperator=*p_seperator;
		*p_seperator='\0';
		g_field[g_nfield++]=p;
		p=p_seperator+1;
	}
	while(seperator==',');
	return g_nfield;
}

/* interface functions */
char *csvGetLine(FILE *fin)
{
	int i,c;
	char *newl,*news;
	if(g_line==NULL){
		g_maxline=g_maxfield=1;
		g_line=(char *)malloc(g_maxline);
		g_sline=(char *)malloc(g_maxline);
		g_field=(char **)malloc(g_maxfield*sizeof(g_field[0]));
		if(g_line==NULL||g_sline==NULL||g_field==NULL){
			reset();
			return NULL;
		}
	}
	for(i=0;(c=getc(fin))!=EOF&&!bEndOfLine(fin,c);++i){
		if(i>=g_maxline-1){
			g_maxline*=2;
			printf("%d ",g_maxline);
			newl=(char *)realloc(g_line,g_maxline);
			printf("%d ",g_maxline);
			printf("#");
			news=(char *)realloc(g_sline,g_maxline);
			printf("#");
			printf("%d*\n",g_maxline);
			if(newl==NULL||news==NULL){
				reset();
				return NULL;
			}
			g_line=newl;
			g_sline=news;
		}
		g_line[i]=c;
	}
	g_line[i]='\0';
	if(split()==NOMEM){
		reset();
		return NULL;
	}
	return (c==EOF&&i==0)?NULL:g_line;
}

char *csvGetField(int n)
{
	if(n<0||n>=g_nfield)
		return NULL;
	return g_field[n];
}

int csvFieldSize(void)
{
	return g_nfield;
}

int main(void)
{
	int i;
	char *line;

	while((line=csvGetLine(stdin))!=NULL){
		printf("line = %s \n",line);
		for(i=0;i<csvFieldSize();++i)
			printf("field[%d] = %s \n",i,csvGetField(i));
	}
	return 0;
}

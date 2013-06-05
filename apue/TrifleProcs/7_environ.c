extern char **environ;
void main()
{
    int i;
    for(i=0;environ[i]!=0;++i)
    	printf("%s\n",environ[i]);
}

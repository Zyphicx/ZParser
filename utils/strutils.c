char *readChars(char *s, char *t, int num){
	while(num--)
		*s++ = *t++;
	*s = '\0';
	return t;
}
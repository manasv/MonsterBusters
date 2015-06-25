//-----Get float----//
float getfloat(){
	int count, lim = 15;
	char arra[lim+1];
	ask:
	for(count = 0; count < lim; count++){
		arra[count] = getchar();
		if(arra[count]=='\n' && count > 0){
			break;
		}else if(arra[count] < 46 || arra[count] > 57){
			arra[0] = 'x';
			break;
		}
	}
	arra[lim+1]='\0';
	if(atof(arra) == 0 && arra[0] == '0'){
		return atof(arra);
	}else if(atof(arra)==0){
		goto ask;
	}else{
		return atof(arra);
	}
}

//-----Get integers----//
int getint(){
			printf("fuck!");
	int count, lim = 15;
	char arra[lim+1];
	ask:
	for(count = 0; count < lim; count++){
		arra[count] = getchar();
		if(arra[count]=='\n' && count > 0){
			break;
		}
	}
	arra[lim+1]='\0';
	if(atoi(arra) == 0 && arra[0] == '0'){
		return atoi(arra);
	}else if(atoi(arra)==0){
		goto ask;
	}else{
		return atoi(arra);
	}
}

//--Get String--//
char* getstr(){
	int i, lim = 10;
	char* str = malloc( lim );
	for(i = 0; i < lim; i++){
		str[i] = getchar();
		if(str[i] == '\n' && i > 0){
			str[i] = '\0';
			break;
		}
	}
	str[i+1] = '\0';
	return str;
}

char* getname(){ //ligeramente modificada para no permitir espacios!
	int i, lim = 10;
	char* str = malloc( lim );
	for(i = 0; i < lim; i++){
		str[i] = getchar();
		if(str[i] == '\n' && i > 0){
			str[i] = '\0';
			break;
		}else if( str[i] == 32){
			str[i] = '_';
		}
	}
	str[i+1] = '\0';
	return str;
}

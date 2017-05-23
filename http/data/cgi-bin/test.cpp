#include <stdio.h>
#include <stdlib.h>
int main() {
	/* выдаем обязательный заголовок Content-type и пустую строку
	перед телом сообщения*/
	/* выдаем текст собственно страницы*/
	printf("<html><body>");
	printf("Приветствую! Вы ввели аргументы:’%s’/n с адреса ’ %s’/n", getenv("QUERY_STRING"), getenv("REMOTE_ADDR"));
	printf("’%s’/n", getenv("REMOTE_ADDR"));
	printf("QUERY_STRING = ’%s’/n", getenv("QUERY_STRING"));
	printf("DOCUMENT_ROOT =’%s’/n", getenv("DOCUMENT_ROOT"));
	printf("SCRIPT_FILENAME ’%s’/n", getenv("SCRIPT_FILENAME"));
	printf("</body></html>");
	return 0;
}

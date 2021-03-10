
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "read_cJSON.h"

void doit(char *pszJsonText)
{
	  cJSON   *root = NULL;
	  cJSON   *data = NULL;
	  cJSON   *val = NULL	;
	
	  char *out = NULL;
	
     //将json数据解析成json结构体
      root= cJSON_Parse(pszJsonText);
			
		  data = cJSON_GetObjectItem(root,"data");
		 
      //从result节点获取各个元素的值
        val =  cJSON_GetObjectItem(data,"cityName");
			out = val->valuestring;
			printf("城市=%s \r\n",out);
	
	    val =  cJSON_GetObjectItem(data,"lastUpdate");
			out = val->valuestring;
			printf("时间=%s \r\n",out);
	
		val =  cJSON_GetObjectItem(data,"tq");
			out = val->valuestring;
			printf("天气=%s \r\n",out);
	
		val =  cJSON_GetObjectItem(data,"qw");
			out = val->valuestring;
			printf("气温=%s \r\n",out);
	
      cJSON_Delete(root);
}




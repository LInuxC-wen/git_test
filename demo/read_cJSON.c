
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "cJSON.h"
#include "read_cJSON.h"
#include "font_test.h"

int read_weather(char *str);

void doit(char *pszJsonText)
{
	  cJSON   *root = NULL;
	  cJSON   *data = NULL;
	  cJSON   *val = NULL;
	
	  char *out = NULL;
	  
     //将json数据解析成json结构体
      root= cJSON_Parse(pszJsonText);
			
		  data = cJSON_GetObjectItem(root,"data");
		 
      //从result节点获取各个元素的值
        val =  cJSON_GetObjectItem(data,"cityName");
			out = val->valuestring;
			printf("城市=%s \r\n",out);
			wchar_t *out_cn =L"广州";
			font_test_cn(120,50,50,out_cn);
	
	    val =  cJSON_GetObjectItem(data,"lastUpdate");
			out = val->valuestring;
			printf("时间=%s \r\n",out);
			font_test(130,180,50,out);
	
		val =  cJSON_GetObjectItem(data,"tq");
			out = val->valuestring;
			printf("天气=%s \r\n",out);
			read_weather(out);
	
		val =  cJSON_GetObjectItem(data,"qw");
			out = val->valuestring;
			printf("气温=%s \r\n",out);
			font_test(180,420,50,out);
	
      cJSON_Delete(root);
}


int read_weather(char *str)
{
char *sunny = "晴";
char *cloudy = "多云";
char *overcast = "阴";
char *shower = "阵雨";
char *thundershower = "雷阵雨";
char *lightrain = "小雨";
char *moderaterain = "中雨";
char *heavyrain = "大雨";

	if(strcmp(str,sunny)==0)
	{
		wchar_t *out_cn =L"晴";
		font_test_cn(150,280,50,out_cn);
	}
	
	else if(strcmp(str,cloudy)==0)
	{
		wchar_t *out_cn =L"多云";
		font_test_cn(150,280,50,out_cn);
	}
	
	else if(strcmp(str,overcast)==0)
	{
		wchar_t *out_cn =L"阴";
		font_test_cn(150,280,50,out_cn);
	}
	
	else if(strcmp(str,shower)==0)
	{
		wchar_t *out_cn =L"阵雨";
		font_test_cn(150,280,50,out_cn);
	}
	
	else if(strcmp(str,thundershower)==0)
	{
		wchar_t *out_cn =L"雷阵雨";
		font_test_cn(150,280,50,out_cn);
	}
	
	else if(strcmp(str,lightrain)==0)
	{
		wchar_t *out_cn =L"小雨";
		font_test_cn(150,280,50,out_cn);
	}
	
	else if(strcmp(str,moderaterain)==0)
	{
		wchar_t *out_cn =L"中雨";
		font_test_cn(150,280,50,out_cn);
	}
	
	else if(strcmp(str,heavyrain)==0)
	{
		wchar_t *out_cn =L"大雨";
		font_test_cn(150,280,50,out_cn);
	}
	
	return 0;
}

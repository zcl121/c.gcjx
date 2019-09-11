#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lrc.h"
#include "./pos/console.h"
extern TIME m_time;
extern char * song_msg[4];//保存前四行歌曲信息首地址
/*************************************************************************
*	函数功能:读出文件内容
*	参数： 
* 		src_file_name:文件名字，从此文件中读取内容。 
*	返回值:读出字符串的首地址
**************************************************************************/
char * read_src_file(char *src_file_name)
{
	unsigned long int FILE_SIZE=0;
	char *src_file_text=NULL;
	FILE  *fp=NULL;
	
	fp=fopen(src_file_name,"rb");
	if(fp==NULL)
	{
		printf("open %s faild\n",src_file_name);
		return 0;
	}
	
	fseek(fp,0,SEEK_END);
	FILE_SIZE=ftell(fp);
	
	rewind(fp);					//一定要把“文件指针”移动到文件开始处
	src_file_text=(char *)malloc(FILE_SIZE+1);
	memset(src_file_text,'\0',FILE_SIZE+1);
	if(fread(src_file_text,FILE_SIZE,1,fp)==0)
	{
		printf("read from src_file faild\n");
		return NULL;
	}
	fclose(fp);					// 关闭文件
	return src_file_text ;
}
/************************************************************************
*	函数功能:切割 歌词数组
*	参数：
*		lrc_text 指针数组，让其指针指向切出来的内容 
* 		file_buf 歌词内容数组
* 	返回值:切割出来的字符串的个数
*************************************************************************/
int strtok_lrc_buf(char *lrc_text[],char *file_buf)
{
	int i=0;
	
	lrc_text[i]=strtok(file_buf,"\r\n");
	while(lrc_text[i]!=NULL)
	{
		lrc_text[++i]=strtok(NULL,"\r\n");
	}
	return i;
}


/**************************************************************************
*	函数功能:解析lrc前四行，歌曲的信息如歌手、专辑等。。
*	参数：
*		lrc_text 指针数组 lrc 中的前四行 
*		song_msg 指针数组 用来指向处理好的歌曲信息
* 	返回值:无
**************************************************************************/
void lrc_head_resolve(char *song_msg[],char * lrc_text[])
{
	int i=0;
	char * message[]={"[ti","[ar","[al","[by"};
	for(i=0;i<4;i++)// 循环四次 找到四个头信息的开始
	{
		if (strncmp(lrc_text[i],message[i],2)==0)
		{
			song_msg[i]=(char *)malloc(strlen(lrc_text[i])-4);
			sscanf(lrc_text[i],"%*4s%[^]]",song_msg[i]);
		}
		else//如果没有这个歌曲信息，song_msg[i]=NULL
		{
			song_msg[i]=NULL;
		}
	}
}

/**************************************************************************
*	函数功能:打印歌曲信息
*	参数：
*		song_msg 歌曲信息 指针数组指向歌曲头信息内容
*	返回值:无
*	注意点：无
**************************************************************************/
void print_song_message(char * song_msg[])
{
	int i=0;
	char *name[4]={"歌名:","歌手:","专辑:","制作:"};
	clear_screen();
	cusor_hide();
	set_fg_color(COLOR_BLUE);
	for(i=0;i<4;i++)
	{
		if(song_msg[i]!=NULL)
		{
			cusor_moveto(35,i+1);
			printf("%s  %s\n",name[i],song_msg[i]);
		}
	}
}


/**************************************************************************
*	函数功能:解析歌曲信息
*	参数：
*		lrc_text 指向歌词内容(以行的方式存储)的指针数组
*		lrc_line 歌词的行数
* 	返回值:无
**************************************************************************/
LRC* lrc_resolve(int lrc_line,char * lrc_text[])
{
	LRC *head=NULL,*p_new=NULL;
	int i=0,j=0;
	char *lrc_str;
	int time_buf[10];		//用来暂时记录歌词中的时间,单位为：毫秒
	int time_num=0;				//用来暂时记录本句歌词有多少个时间与其对应
	int min,sec;
	for(i=4;i<lrc_line;i++)
	{
		lrc_str=lrc_text[i];//一行的歌词：[02:04.94][00:36.09]我想大声宣布 对你依依不舍
		time_num=0;		//解析每句歌词时，要把本句歌词对应的时间数初始化为0
		while(*lrc_str == '[')//解析一行的所有时间
		{
			sscanf(lrc_str,"[%2d:%2d",&min,&sec);
			time_buf[time_num++]=min*60+sec;
			lrc_str=lrc_str+10;
		}//解析完一行的所有时间后 lrc_str即为歌词的首地址
		//根据本句歌词对应的时间个数，把歌词与时间 保存到链表中
		for(j=0;j<time_num;j++)
		{
			p_new =  ( LRC *)malloc(sizeof(LRC)); //申请节点，存放解析出歌词与时间
			p_new->time = time_buf[j];
			strcpy (p_new->lrc_buf,lrc_str);	
			double_link_insert(&head,p_new);//将新来的节点pi插入到链表中
		}
	}
	return head;
}

LRC* link_search(LRC *head,int m_time)
{
	LRC *pb=head;
	while(pb!=NULL)
	{
		if(pb->time==m_time)
			return pb;
		else
			pb=pb->next;
	}
	return NULL;
}


void double_link_insert(LRC **p_head,LRC *p_new)
{
	LRC *pb,*pf;
	pb=*p_head;
	if(*p_head == NULL)//链表为空，新来的节点就是头节点
	{
		*p_head=p_new;
		p_new->front=NULL;
		p_new->next=NULL;
		return ;
	}
	while((p_new->time >= pb->time) && (pb->next!=NULL) )
	{
		pb=pb->next ;
	}
	if(p_new->time < pb->time)//找到了一个pb的num比新来的节点的num大，插在pb前边
	{
		if(pb==*p_head)//找到的节点是头节点，插在头节点的前边
		{
			p_new->next=*p_head;
			(*p_head)->front=p_new;
			p_new->front=NULL;
			*p_head=p_new;
		}
		else
		{
			pf=pb->front;//pf指向 找到节点的前一个节点
			
			p_new->next=pb;
			p_new->front=pf;
			pf->next=p_new;
			pb->front=p_new;
		}
	}
	else//所有pb指向节点的num都比p_new指向的节点的num小，插在最后
	{
		pb->next=p_new;
		p_new->front=pb;
		p_new->next=NULL;
	}
}



/**************************************************************************
*	函数功能:回收链表
*	参数：
*		head 链表的头节点地址。   
*		
* 	返回值:无
**************************************************************************/
void link_free(LRC *head)
{
	LRC *pb;
	while(head!=NULL)
	{
		pb=head;
		head=head->next;
		free(pb);
	}
}
/************************************************************************
*	函数功能:延时函数
*	参数：
*		sec ：延时的秒数
* 	返回值:无
*************************************************************************/
void time_delay(int sec)
{
	int s_time,e_time;
	s_time=time(NULL);
	while(1)
	{
		e_time=time(NULL);
		if(e_time==s_time+sec)
			break;
	}
}


/************************************************************************
*	函数功能:在屏幕的x,y位置，用color颜色打印一个字符串str
*	参数：
*		str：要打印的字符串的首地址
		x:显示在屏幕上的x坐标
		y:显示在屏幕上的y坐标
		color: 显示字符串的颜色
* 	返回值:无
*************************************************************************/
void color_pos_printf(char *str,int x,int y,int color)
{
			cusor_moveto(x,y);
			set_fg_color(color);
			printf("%s\n",str);
}
/************************************************************************
*	函数功能:在屏幕上打印时间
*	参数：
*		m_time: 时间结构体
* 	返回值:无
*************************************************************************/
void  time_printf(TIME m_time)
{
	char str[30];
	sprintf(str,"%02d:%02d",m_time.minute,m_time.second);
	color_pos_printf(str,40,5,COLOR_BLUE);
	//cusor_moveto(0,6);
}


/************************************************************************
*	函数功能:打印链表中5个节点的信息到屏幕上
*	参数：
*		start_point:打印的第一个节点
		pb: 要高亮的节点
* 	返回值:无
*************************************************************************/
void lrc_printf(LRC *start_point , LRC *pb)
{
	int i;
	for(i=0;i<LINE_TOTAL;i++)//从pb开始打印5句歌词 
	{
		if(start_point==pb)//高亮红色显示节点
			color_pos_printf(start_point->lrc_buf,30,6+i,COLOR_RED);
		else if(start_point!=NULL)//普通蓝色显示
			color_pos_printf(start_point->lrc_buf,30,6+i,COLOR_BLUE);
		if(start_point!=NULL)	
			start_point=start_point->next;
	}
}

/*
	功能，在head指向的链表中， pb指向正在唱的节点，
	找要打印的5个节点的首节点的地址，然后返回其地址
*/
LRC * find_start_point(LRC *head,LRC*pb)
{
#if 1
	int i;
	for(i=0;i<LINE_TOTAL/2;i++){
		if(pb==head)
			return head;
		else
			pb = pb->front;
	}
	return pb;
#else	
	if(pb==head)//第1个节点
		return head;
	else if((pb->front)==head)//第2个节点
		return head;
		
	//判断 最后面几个节点，如果是，返回倒数第5个节点的地址
	else if(pb->next==NULL)
		return (((pb->front)->front)->front)->front;
	else if(pb->next->next==NULL)
		return (((pb->front)->front)->front);
	else
		return (pb->front)->front;		
#endif		
}


void m_clock(TIME *p_m_time)
{
		
		(*p_m_time).second++;
		if((*p_m_time).second==60)
		{
			(*p_m_time).second=0;
			++(*p_m_time).minute;
			if((*p_m_time).minute==60)
				(*p_m_time).minute=0;
		}
}


void song_msg_free(char *song_msg[])
{
	int i;
	for(i=0;i<4;i++)
	{
		if(song_msg[i]!=NULL)
			free(song_msg[i]);
	}
}








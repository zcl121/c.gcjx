#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "console.h"
#include "start_mplayer.h"
extern void set_bg_color(int color);
extern void set_fg_color(int color);
extern void mplayer_play(char * song_path);
extern void time_delay(int sec);


typedef  struct lrc{
	int m_time;
	char lrc_buf[200];
	struct lrc *front;
	struct lrc *next;
}LRC;

extern LRC *link_insert(LRC *head,LRC *pnew);
extern LRC *link_find(LRC *head,int m_time);


int msg_deal(char *msg_src,char *msg_done[],char *str)
{
	int i=0;
	msg_done[i]=strtok(msg_src,str);
	while(msg_done[i]!=NULL)
	{
		i++;
		msg_done[i]=strtok(NULL,str);
	}
	return i;
}



int main()
{
	int time_buf[5];
	int i=0,j,k,temp=0;
	char *str;
	int min,sec;
	LRC *head=NULL,*p_new,*pb;
	char *q[100];
	int len=0;
	FILE *fp;
	char *p;
	int leng,ret;
	clear_screen(); 
				fp=fopen("./歌词/止战之殇.lrc","rb");
				fseek(fp,0,SEEK_END);
				leng=ftell(fp);
				
		       char lrc_buf[leng+1];
	  
				p=malloc(leng+1);
				rewind(fp);
				
				fread(p,1,leng,fp);
				p[leng]='\0';
				strcpy(lrc_buf,p)	;	

	len=msg_deal(lrc_buf,q,"\r\n");
					
	for(k=4;k<len;k++)//处理第5行到最后一行
	{
		str=q[k];
		i=0;
			while(*str=='[')
			{
				min=atoi(str+1);
				sec=atoi(str+4);
				time_buf[i++]=min*60+sec;
				str=str+10;
			}
			for(j=0;j<i;j++)
			{
				p_new = malloc(sizeof(LRC));//申请节点
				p_new->m_time=time_buf[j];//节点的时间成员赋值
				strcpy(p_new->lrc_buf,str);//节点的歌词成员赋值
				head=link_insert(head,p_new);//插入链表
			}
	}
		
	 mplayer_play("./歌曲/止战之殇.mp3");
	  usleep(2500*1000);
	int minute,second,count=0;
	
    minute = 00;
    second = 00;
		int l=1;
					   
	
	
    while(1)
    {	
					for(k=0;k<4;k++)  	// 前四行
						{
							str=q[k];
							sscanf(str,"%*[^:]%*c%[^]]",str);
							cusor_moveto(53,k+1);
							set_fg_color(37);
							printf("%s\n",str);
						}

					
	
	
		
       cusor_hide();
      
	   
        second++;
        if(second >= 60)
        {
            second = 0;	
            minute++;
            if(minute >= 60)
            {
                minute = 0;
            }
        }
	
	
	cusor_moveto(54,6);
	sleep(1);
	printf("%02d:%02d",minute,second);
	cusor_get_pos();
			 
		temp++;
		p_new=link_find(head,temp);
		
	
			
		
		
		if(p_new != NULL){
		
			
		clear_screen();       //让歌词不会互相覆盖
		cusor_moveto(54,6);
		printf("%02d:%02d",minute,second);  //时间不闪烁
				
				
		
				if(p_new==head)
				{
				
					set_fg_color(37);
					cusor_moveto(45,7);
					printf("%s\n",p_new->lrc_buf);
					
					
				  }
				 else if(p_new->front==head) 
				 {
					set_fg_color(37);
					cusor_moveto(45,7);
					printf("%s\n",p_new->front->lrc_buf);
					 set_fg_color(37);
					 cusor_moveto(45,8);
					printf("%s\n",p_new->lrc_buf);
				 
				 }
				else if(p_new->front!=head){
				set_fg_color(37);
				cusor_moveto(45,7);
				printf("%s\n",p_new->front->front->lrc_buf);
				
				set_fg_color(37);
				cusor_moveto(45,8);
				printf("%s\n",p_new->front->lrc_buf);
				
			    set_fg_color(32);
			    cusor_moveto(45,9);
				printf("%s\n",p_new->lrc_buf);
				
				  if(p_new->next==head)
							break;
				
			   set_fg_color(37);
			   cusor_moveto(45,10);
			   printf("%s\n",p_new->next->lrc_buf);
			   
			  set_fg_color(37);
			  cusor_moveto(45,11);
			  printf("%s\n",p_new->next->next->lrc_buf);
					  
			  set_fg_color(37);
			  cusor_moveto(45,12);
			  printf("%s\n",p_new->next->next->next->lrc_buf);
			  set_fg_color(37);
			   cusor_moveto(45,13);
			  printf("%s\n",p_new->next->next->next->next->lrc_buf);
			
				
		
			}
			
			}
		
		

		
        
    }

   
 
	
	
	fclose(fp);
	free(p);
	
	return 0;
}

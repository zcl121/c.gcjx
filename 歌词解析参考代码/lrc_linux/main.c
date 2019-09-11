#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "lrc.h"
#include "start_mplayer.h"
#include "./pos/console.h"
TIME m_time={0,0};
char *song_msg[4];//保存前四行歌曲信息首地址
int main(int argc,char *argv[])
{
	char * lrc_str;
	char * lrc_text[200]={NULL};//指针数组保存每一行的地址
	int  line_num,s_time=0;
	LRC *head=NULL,*pb=NULL;
	LRC *start_point;//start_point 为要打印的5个节点的首节点												//存储模拟时间
	lrc_str=read_src_file("./lrc/简单爱.lrc");//把lrc文件中的歌词内容读出来
	line_num=strtok_lrc_buf(lrc_text,lrc_str);//切割lrc_str ，每一行的地址保存在lrc_text中
	lrc_head_resolve(song_msg,lrc_text);//处理lrc文件的前四行
	head=lrc_resolve(line_num,lrc_text);//所有时间点都对应一个节点存放在链表中
	free(lrc_str);//回收之前申请的内存（从文件中读取数据存放的字符数组）
	
	print_song_message(song_msg);//打印歌曲信息
	time_printf(m_time);//打印时间
	lrc_printf(head,NULL);//打印前5句歌词，都不高亮
	
	while(1)//最后一个节点了退出循环
	{
		time_printf(m_time);//打印时间
		pb=link_search(head,s_time);//在链表中查找节点
		if(pb!=NULL)//找到节点显示
		{
			start_point=find_start_point(head,pb);//start_point 为开始打印的节点，pb为正在唱的节点
			print_song_message(song_msg);//打印歌曲信息
			time_printf(m_time);//打印时间
			lrc_printf(start_point,pb);//从start_point开始打印五个节点的信息,pb是要高亮显示的节点
			if(pb->next==NULL)//如果找到的节点是尾节点，退出程序
				break;
		}
		
		usleep(1000*100);
		s_time=s_time+1;
		m_clock(&m_time);//时钟成员(分钟min和秒sec)赋值
	}
	link_free(head);//释放链表
	song_msg_free(song_msg);//释放歌曲信息
	cusor_show();//显示光标
	return 0;
}
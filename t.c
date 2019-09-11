#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "console.h"
#include "start_mplayer.h"
extern void mplayer_play(char * song_path);

main(){
 mplayer_play("简单爱.mp3");

}
#include <string.h>
#include <stdint.h>
//#include <math.h>

#include "lib/blitter/blitter.h"
#include "lib/sampler/sampler.h"

#include "ninja_bg.h"
#define TMAP_INTRO_W 40
#define TMAP_INTRO_H 30
#define TILE_SIZE 16

#define SMP(x) extern const int8_t sample_##x##_data[]; extern const int sample_##x##_len;extern const int sample_##x##_rate;
#define PLAY(x) play_sample(sample_##x##_data,sample_##x##_len, 256*sample_##x##_rate/BITBOX_SAMPLERATE,-1, 50,50 );
#define Max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
     
#define Min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

SMP(intro);

enum Color { black, red, green, blue };

extern const uint16_t ninja_bg_tset[]; // share tileset

uint16_t intro_ramtilemap[TMAP_INTRO_H][TMAP_INTRO_W];
int start_frame;
int anim_start_frame;
int saw_Frame;

static int cursor;
int speed;
int fr_count = 0;
int fr_SawCount = 0;
int fr_SawCount_2 = 0;
int ninjaJump = 0;
int ninjaLeft = 0;
int ninjaRight = 0;
int ninjaFlip = 0;
int ninjaUp = 0;
int ninjaDown = 0;
int collideLeft = 0;
int collideRight = 0;
int collideUp = 0;
int collideDown = 0;
int collideObstacle = 0;
int ninjaPrevX = 0;
int ninjaPrevY = 0;
int countJump = 0;
int ninjaSpeed = 1;
float gravity = 0;
int collideSaw = 0;
int collideSaw_2 = 0;
int ninjaDead = 0;
int scanToRespawm = 0;
int sawDiscForward = 0;

extern void (*do_frame)(void);

extern  void level_init(int);
extern char ninja_spr[];
extern char sawDisc_1_spr[];

static struct object *bg, *ninja, *sawDisc, *sawDisc_2; // tilemap

enum TileType {
	tile_empty=0,   // don't block or anything
	tile_standard,  // standard, colored or invisible. color match = jump/score, else : jump petit
	tile_superjump, // color match : super+, else : normal
	tile_light,     // color match : jump, else : don't
	tile_blocking,  // color match : passe, sinon bloque
	tile_new_color, // add a color to possible colors
	tile_restart_point, // color match: yes, else : no!
	tile_left,      // color : normal, sinon : envoie a G
	tile_right, 	   // color : normal, sinon : envoie a D
	tile_killing   // color : normal, sinon : tue !
};

void intro_out();

/*int collide(ACTOR *aa, ACTOR *ab)
{
    if (aa->x+(aa->w) < ab->x ||
        aa->y+(aa->h) < ab->y ||
        aa->x > ab->x+ab->w ||
        aa->y > ab->y+ab->h) {
        return 0;
    } else {
    	return 1;
    }
}*/

int collideTile(object *aa, int t_x, int t_y)
{
    if (aa->x+(aa->w) < t_x ||
        aa->y+(aa->h) < t_y ||
        aa->x > t_x+TILE_SIZE ||
        aa->y > t_y+TILE_SIZE) {
        return 0;
    } else {
    	return 1;
    }
}



static inline enum TileType tile_type_from_id(uint16_t tile_id)
{
	if (tile_id>=tile_killing*16)
		return tile_empty;
	else if (1+(tile_id-1)/16 == 6)
		// fakes !
		return tile_empty;
	else
		return 1+((tile_id-1)/16);
}


static inline enum Color tile_color_from_id(uint16_t tile_id)
{
	if (tile_id>tile_killing*16)
		return black; // whatever, they're empty
	else {
		//printf("tile %d col %d\n", tile_id,(tile_id%16)/4 );
		return ((tile_id-1)%16)/4;
	}
}

void testTiles()
{

    
    collideTile(ninja,ninja->x,ninja->y);
    /*int t_x = ninja->x / TILE_SIZE;
    int t_y = ninja->y / TILE_SIZE;    
    int dest_tile = ninja_bg_tmap[0][t_y * (bg->w / TILE_SIZE)+ t_x];
    
    int tileLeft = ninja_bg_tmap[0][(ninja->y / TILE_SIZE) * (bg->w / TILE_SIZE) + t_x];
    int tileRight = ninja_bg_tmap[0][t_y * (bg->w / TILE_SIZE) + t_x+1];
    int tileUp = ninja_bg_tmap[0][(t_y) * (bg->w / TILE_SIZE) + t_x];
    int tileDown = ninja_bg_tmap[0][(t_y+2) * (bg->w / TILE_SIZE)+ t_x];
    int tilecolor = tile_color_from_id(dest_tile);   
    
    if(tileRight != 49) collideRight = 1;
    else collideRight = 0;
    if(tileLeft != 49) collideLeft = 1;
    else collideLeft = 0;
    if(tileUp != 49) collideUp = 1;
    else collideUp = 0;
    if(tileDown != 49) collideDown = 1;
    else collideDown = 0;*/
    
    /*if(dest_tile == 49){
         collideObstacle = 0;
         ninjaPrevX = ninja->x;
         ninjaPrevY = ninja->y;
         collideDown = 0;
         collideLeft = 0;
         collideRight = 0;
         collideUp = 0;
    }
    else
    {*/
        
        /*if(((ninja->x + 16) / TILE_SIZE)+1  > ninjaPrevX) 
        {
            collideRight = 1;            
        }*/
        /*else if(t_x < ninjaPrevX)
        {
            collideLeft = 1;
        }*/
        //if(ninjaPrevTileY < t_y) collideDown = 1;
        //else if(ninjaPrevTileY > t_y) collideUp = 1;
        //if(ninjaPrevTileX < t_x) collideRight = 1;
        //if(ninjaPrevTileX < t_x) collideRight = 1;
    //}
    
    
    
    /*printf("ninja X: %d, ", ninja->x);
    printf(" ninja Y: %d, ", ninja->y);    
    printf("tile X: %d, ", t_x);
    printf("tile Y: %d, ", t_y);
    printf("tile L: %d, ", tileLeft);
    printf("tile R: %d, ", tileRight);
    printf("tile U: %d, ", tileUp);
    printf("tile D: %d, ", tileDown);
    printf(" tile ID: %d, \n", dest_tile);*/
    
    
    /*if(dest_tile == 17)
    {
        collideLeft = 1;
    }else collideLeft = 0;
    
    if(dest_tile == 19)
    {
        collideRight = 1;
    }else collideRight = 0;*/
    
    
    
    
	/*switch(tile_type_from_id(dest_tile))
	{
		case tile_empty :
		break;
    }*/
    
}

float collideCircle(int CircleX,int CircleY, int CircleRadius, int RectX, int RectY, int RectWidth, int RectHeight)
{    
    //printf("Radius: %d\n", CircleRadius);
    float DeltaX = CircleX - Max(RectX, Min(CircleX, RectX + RectWidth));    
    float DeltaY = CircleY - Max(RectY, Min(CircleY, RectY + RectHeight));
    //printf("DeltaX: %06f\n",DeltaX);
    //printf("DeltaY: %06f\n",DeltaY);
    return (DeltaX * DeltaX + DeltaY * DeltaY) < (CircleRadius * CircleRadius);
}

void moveSaw(){
    if(sawDisc->x > 100 && !sawDiscForward) sawDisc->x--;
    else sawDiscForward = 1;
    if(sawDisc->x < 400 && sawDiscForward) sawDisc->x++;
    else sawDiscForward = 0;
    
}

void moveNinja(){
    int dest_tile;
    if(ninjaLeft) {
        if(collideRight) 
        {
           countJump = 0;
            collideRight = 0;
        }    
        ninja->x -= 3 * ninjaSpeed;
        
        dest_tile = ninja_bg_tmap[0][ninja->y/TILE_SIZE * (bg->w / TILE_SIZE) + ninja->x/TILE_SIZE];
        //dest_tile = ninja_bg_tmap[0][(ninja->x/TILE_SIZE) * (bg->w / TILE_SIZE) + ninja->x/TILE_SIZE];
        
        if(dest_tile != 49) 
        {
            ninja->x += 3 * ninjaSpeed;
            collideLeft = 1;
            //countJump = 30;            
        }else {        
            dest_tile = ninja_bg_tmap[0][(ninja->y + ninja->h)/TILE_SIZE * (bg->w / TILE_SIZE) + ninja->x/TILE_SIZE];
            //dest_tile = ninja_bg_tmap[0][(ninja->y + ninja->h)/TILE_SIZE * (bg->w / TILE_SIZE) + (ninja->x+ninja->w)/TILE_SIZE];
        
            if(dest_tile != 49) 
            {
                ninja->x += 3 * ninjaSpeed;
                collideLeft = 1;
                //countJump = 30;            
            }
        }
    }else if(ninjaRight) {
        if(collideLeft) 
        {
            countJump = 0;
            collideLeft = 0;
        }    
        ninja->x += 3 * ninjaSpeed;        
        
        dest_tile = ninja_bg_tmap[0][ninja->y/TILE_SIZE * (bg->w / TILE_SIZE) + (ninja->x+ninja->w)/TILE_SIZE];
        
        if(dest_tile != 49) 
        {
            ninja->x -= 3 * ninjaSpeed;
            collideRight = 1;
            //countJump = 30;            
        }else {        
            dest_tile = ninja_bg_tmap[0][(ninja->y + ninja->h)/TILE_SIZE * (bg->w / TILE_SIZE) + (ninja->x+ninja->w)/TILE_SIZE];
        
            if(dest_tile != 49) 
            {
                ninja->x -= 3 * ninjaSpeed;
                collideRight = 1;
               // countJump = 30;            
            }
        }
    }
    
    if(ninjaUp && countJump < 20) {
        /*if(collideDown) 
        {
            countJump = 0;
            //collideUp = 0;
        }*/
        countJump++;  
          
        ninja->y -= 4;
        
        dest_tile = ninja_bg_tmap[0][ninja->y/TILE_SIZE * (bg->w / TILE_SIZE) + ninja->x/TILE_SIZE];        
        //ninja_bg_tmap[0][0] = 49;
        
        if(dest_tile != 49) 
        {
            ninja->y += 4;
            collideUp = 1;
            countJump = 30;            
        }else {
            dest_tile = ninja_bg_tmap[0][ninja->y/TILE_SIZE * (bg->w / TILE_SIZE) + (ninja->x + ninja->w) /TILE_SIZE];        
            if(dest_tile != 49) 
            {
                ninja->y += 4;
                collideUp = 1;
                countJump = 30;            
            }
        }
        
    }else if(ninjaDown) {
        if(collideUp) 
        {
            //countJump = 0;
            //collideDown = 0;
        }    
        ninja->y += gravity;      
        
        
        dest_tile = ninja_bg_tmap[0][(ninja->y + ninja->h)/TILE_SIZE * (bg->w / TILE_SIZE) + ninja->x/TILE_SIZE];
        
        if(dest_tile != 49) 
        {
            ninja->y -= gravity;
            collideDown = 1;
            gravity = 0;
            if(!ninjaUp) countJump = 0;
        }else {
            dest_tile = ninja_bg_tmap[0][(ninja->y + ninja->h)/TILE_SIZE * (bg->w / TILE_SIZE) + (ninja->x+ninja->w)/TILE_SIZE];
            if(dest_tile != 49) 
            {
                ninja->y -= gravity;
                collideUp = 1;
                gravity = 0;
                if(!ninjaUp) countJump = 0;            
            }
        }
        if(gravity < 10) gravity += 1;      
    }
}

void intro_frame()
{
	if (bg->y<0) { // scroll down : ease w/ overshot ? 
		speed += 1;
		bg->y+= speed;	
		if (bg->y>0) {
			bg->y=0;	
			PLAY(intro);
		}
	} else {	
            //testTiles();	            
			if (GAMEPAD_PRESSED(0,right)) {	                
                ninjaRight = 1;
                ninjaLeft = 0;
                ninjaFlip = 0;                  
			}else if (GAMEPAD_PRESSED(0,left)) {
                ninjaRight = 0;
                ninjaLeft = 1;
				ninjaFlip = 1;             
			}else 
            {
                ninjaRight = 0;
                ninjaLeft = 0;
            }

			if (GAMEPAD_PRESSED(0,A)) {                               
               ninjaUp = 1;			
			}else {//if (GAMEPAD_PRESSED(0,down)) {
                ninjaUp = 0;
                ninjaDown = 1;                
			}
            if (GAMEPAD_PRESSED(0,B)){
                ninjaSpeed = 2;
            }else ninjaSpeed = 1;
        
        if(!ninjaDead)
        {
        if((ninjaRight || ninjaLeft) && (anim_start_frame > 5) && (!ninjaUp))
        {             
            if(ninjaFlip)
            {
                if(fr_count < 16 && fr_count > 12) fr_count++;
                else fr_count = 13;            
                ninja->fr=fr_count;
                anim_start_frame = 0;                
            }
            else
            {
                if(fr_count < 12 && fr_count > 8) fr_count++;
                else fr_count = 9;            
                ninja->fr=fr_count;
                anim_start_frame = 0;
            }
        }else if(anim_start_frame > 3 && (ninjaUp && countJump < 20))
        {
            if(fr_count < 8 && fr_count > 3) fr_count++;
            else fr_count = 4;            
            ninja->fr=fr_count;
            anim_start_frame = 0;                
            
        }else if(anim_start_frame > 10)
        {            
            if(ninjaFlip)
            {
                if(fr_count < 3) fr_count++;
                else fr_count = 2;
                ninja->fr=fr_count;            
                anim_start_frame = 0;
            }else {
                if(fr_count < 1) fr_count++;
                else fr_count = 0;
                ninja->fr=fr_count;            
                anim_start_frame = 0;
            }
        } 
        moveNinja();
    } else
    {
        if(fr_count > 21 || fr_count < 16) fr_count = 17;          
        if(anim_start_frame > 10)
        {      
            
            if(fr_count < 21 && fr_count > 16) fr_count++;
            else {
                //fr_count = 17;            
                ninja->x = 600;
                ninja->y = 40;
                ninjaDead = 0;   
                if(collideSaw) collideSaw = 0;  
                if(collideSaw_2) collideSaw_2 = 0;  
                
            }
            ninja->fr=fr_count;
            anim_start_frame = 0;          
        }
        
    } 
        
            
        
        //printf("test");
        //SDL_Log();
        if(saw_Frame < 1) 
        {
            saw_Frame++;
            moveSaw();
        }
        else
        {
            if(!collideSaw)
            {
                if(fr_SawCount < 4) fr_SawCount++;
                else fr_SawCount = 0;             
                sawDisc->fr=fr_SawCount;
                saw_Frame = 0;
            }else 
            {
                if(fr_SawCount < 9) fr_SawCount++;
                else fr_SawCount = 5;             
                sawDisc->fr=fr_SawCount;
                saw_Frame = 0;
            }
            if(!collideSaw_2)
            {
                if(fr_SawCount_2< 4) fr_SawCount_2++;
                else fr_SawCount_2 = 0;             
                sawDisc_2->fr=fr_SawCount_2;
                saw_Frame = 0;
            }else 
            {
                if(fr_SawCount_2 < 9) fr_SawCount_2++;
                else fr_SawCount_2 = 5;             
                sawDisc_2->fr=fr_SawCount_2;
                saw_Frame = 0;
            }
            
        }
        
        
        
        //printf("Collide Circle: %0.6f\n", 
        if(collideCircle(sawDisc->x + (sawDisc->w/2), sawDisc->y + (sawDisc->h/2), sawDisc->w/2,
                        ninja->x,ninja->y,ninja->w,ninja->h))
                        {
                            collideSaw = 1;
                        }
        if(collideCircle(sawDisc_2->x + (sawDisc_2->w/2), sawDisc_2->y + (sawDisc_2->h/2), sawDisc_2->w/2,
                        ninja->x,ninja->y,ninja->w,ninja->h))
                        {
                            collideSaw_2 = 1;
                        }
        if (collideSaw || collideSaw_2)  ninjaDead = 1;
        
        anim_start_frame++;    
	}
	if (GAMEPAD_PRESSED(0,start)) intro_out();
}

void intro_init(int score, int nbcoins)
{
	cursor = 0;
	speed = 0;	

	bg = tilemap_new(
		ninja_bg_tset,640,480, 
		TMAP_HEADER(TMAP_INTRO_W,TMAP_INTRO_H,TSET_16,TMAP_U8), 
		intro_ramtilemap
		);

	bg->y = -280; // offline
    
    ninja = sprite_new(ninja_spr, 600, 40,0);
	ninja->fr=0;	
    
    sawDisc = sprite_new(sawDisc_1_spr,0,0,0);
    sawDisc->fr=0;
    sawDisc->x= (ninja_bg_saw[0][0] + TILE_SIZE/2) - sawDisc->w/2; // /2 para que el centro coincida    
    sawDisc->y= (ninja_bg_saw[0][1] - TILE_SIZE/2) - sawDisc->h/2; // /2 para que el centro coincida    
    
    sawDisc_2 = sprite_new(sawDisc_1_spr,0,0,0);
    sawDisc_2->fr=0;
    sawDisc_2->x= (ninja_bg_saw[1][0] + TILE_SIZE/2) - sawDisc_2->w/2; // /2 para que el centro coincida    
    sawDisc_2->y= (ninja_bg_saw[1][1] - TILE_SIZE/2) - sawDisc_2->h/2; // /2 para que el centro coincida    

	start_frame=vga_frame;
	
	tmap_blit(bg,0,0,ninja_bg_header,ninja_bg_tmap);

	do_frame = intro_frame;
}

void intro_out()
{
	// de-allocate objects	
	blitter_remove(bg);
    blitter_remove(ninja);
    blitter_remove(sawDisc);	
}

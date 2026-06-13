#include "SpriteSheet.h"


Sprite::Sprite()
{
	image=NULL;
}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
void Sprite::InitSprites(int width, int height)
{
	x = 80;
	y = 35;

	//need to check if dir == something for jumping
	//maxFrame = 12
	//new if statement in draw function for jumping
	maxFrame = 12;
	curFrame = 0;
	jumpFrame = 8;
	jumpFrameDelay = 6;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 50;
	frameHeight = 64;
	animationColumns = 8;
	animationDirection = 1;

	image = al_load_bitmap("guy.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	if(dir == 1){ //right key
		animationDirection = 1;
		goingLeft = false;
		x+=2; 
		if (++frameCount > frameDelay)
		{
			frameCount=0;
			if (++curFrame > maxFrame-5)
				curFrame=1;
		}
	} else if (dir == 0){ //left key
		animationDirection = 0;
		goingLeft = true;
		x-=2; 
		if (++frameCount > frameDelay)
		{
			frameCount=0;
			if (++curFrame > maxFrame-5)
				curFrame=1;
		}

	} else if (dir == 4) { //spacebar
		//jumpFrame = 8;
		animationDirection = 4;
		isJumping = true;
		//curFrame++;
		if (++frameCount > jumpFrameDelay) {
			frameCount = 0;
			if (++jumpFrame > maxFrame-1)
				jumpFrame = 8;
		}
		
	}
	else { //represent that they hit the space bar and that mean direction = 0
		goingLeft = false;
		animationDirection = dir;
		//isJumping = false;
	}
	//check for collided with foreground tiles
	if (animationDirection==0)
	{ 
		if (collided(x, y + frameHeight)) { //collision detection to the left
			x = oldx; 
			y= oldy;
		}

	}
	else if (animationDirection ==1)
	{ 
		if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right
			x = oldx; 
			y= oldy;
		}
	}
	else if (animationDirection == 4) {
		if (collided(x, y - frameHeight)) {
   			x = oldx;
			y = oldy;
		}
	}
}

bool Sprite::CollisionEndBlock()
{

	if (endValue(x + frameWidth/2, y + frameHeight + 5))
		return true;
	else
		return false;
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;
	int jumpx = (jumpFrame % animationColumns) * frameWidth;
	int jumpy = (jumpFrame / animationColumns) * frameHeight;
	
	if (animationDirection == 1) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
	else if (animationDirection == 0) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}
	else if (animationDirection == 2) {
		al_draw_bitmap_region(image, 0, 0, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);

	}
	else if (animationDirection == 4) {
		if (goingLeft) {
			al_draw_bitmap_region(image, jumpx, jumpy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
		}
		else {
			al_draw_bitmap_region(image, jumpx, jumpy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
		}
	}
	//draw when he is jumping. Left and Right jumping
	//else {
	//	al_draw_bitmap_region(image, frameWidth, frameHeight, maxFrame,)
	//}
}

int Sprite::jumping(int jump, const int JUMPIT)
{
	//curFrame++;
	//handle jumping
	if (jump==JUMPIT) { 

		if (!collided(x + frameWidth/2, y + frameHeight + 5))
			jump = 0; 
		
	}
	else
	{
		if ((collideWithCeiling(x+frameWidth/2, y - (jump / 3)) || (collideWithCeiling(x + frameWidth / 2, y - (jump / 3)) || (collideWithCeiling(x + frameWidth/2, y - (jump / 3))))))
		{
			jump--;
		}
		else {
			y -= jump / 3;
		}

		jump--;
		UpdateSprites(900, 480, 4);
	}

	if (jump<0) 
	{ 
		if (collided(x + frameWidth/2,  y + frameHeight))
		{ 
			jump = JUMPIT; 
			while (collided(x + frameWidth/2,y + frameHeight))
			{
				y -= 3;

			}
		} 
	}
	return jump;
}
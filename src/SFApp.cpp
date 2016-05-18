#include "SFApp.h"
#include <unistd.h>

//initializing all my int's from the SFAPP.h
SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), score(0), counter(0), alienCounter(0), health1(25), coinCounter(0), print1(0), is_running(true), sf_window(window) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

//Before anything is done I print the Rules to the terminal.
std::cout << "" << std::endl;
std::cout << "1)If any Aliens collide with the Player = Game Over!" << std::endl;
std::cout << "2)If any Aliens collide with the Walls = Game Over!" << std::endl;
std::cout << "3)If any Coins collide with the Walls = Game Over" << std::endl;
std::cout << "4)If the Alien Boss collides with any Walls = Game Over!" << std::endl;
std::cout << "5)If the Alien Boss collides with the Player = Game Over!" << std::endl;
std::cout << "6)If 5 Objects are Destroyed = Double Points!" << std::endl;
std::cout << "" << std::endl;

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2,100);
  player->SetPosition(player_pos);

//Number of Alien bosses to be rendered.
//Print out what they are worth (score) to the terminal.
 const int number_of_alienBosses = 1;
 for(int i=0; i<number_of_alienBosses; i++) {
 auto alienBoss  = make_shared<SFAsset>(SFASSET_ALIENBOSS, sf_window);
  auto alienBoss_pos = Point2((canvas_w/number_of_alienBosses) * (i+0.5), 530.0f);
  alienBoss->SetPosition(alienBoss_pos);
  alienBosses.push_back(alienBoss);
if(i == 0 ){
std::cout << "Alien End Boss    = 30 Points!" << std::endl;
}
 }

//Number of Aliens to be rendered.
//Print out what they are worth (score) to the terminal.
  const int number_of_aliens = 6;
  for(int i=0; i<number_of_aliens; i++) {
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
    auto pos   = Point2((canvas_w/number_of_aliens) * (i+0.5), 320.0f);
    alien->SetPosition(pos);
    aliens.push_back(alien);
if(i == 0 ){
std::cout << "Alien             = 10 Points!" << std::endl;
} 
  }

//Number of walls to be rendered.
const int number_of_wallsx = 20;
  for(int i=0; i<number_of_wallsx; i++) {
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2((canvas_w/number_of_wallsx) * i, 0.0f);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }

//Number of walls to be rendered.
const int number_of_wallsx2 = 20;
  for(int i=0; i<number_of_wallsx2; i++) {
    auto wall = make_shared<SFAsset>(SFASSET_WALL2, sf_window);
    auto pos   = Point2((canvas_w/number_of_wallsx2) * i, 480.0f);
    wall->SetPosition(pos);
    walls2.push_back(wall);
  }

//Number of walls to be rendered.
const int number_of_wallsy = 20;
  for(int i=0; i<number_of_wallsy; i++) {
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2(0.0f, (canvas_w/number_of_wallsy) * i);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }


//Number of walls to be rendered.
const int number_of_wallsy2 = 20;
  for(int i=0; i<number_of_wallsy2; i++) {
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2(640.0f, (canvas_w/number_of_wallsy2) * i);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }

//Number of walls to be rendered.
const int number_of_wallsHorizontal = 5;
  for(int i=0; i<number_of_wallsHorizontal; i++) {
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2((canvas_w/number_of_wallsHorizontal) * i, 120.0f);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }
  
//Number of bombs to be rendered.
//Print out what they are worth (score) to the terminal.
const int number_of_bombs = 3;
  for(int i=0; i<number_of_bombs; i++) {
    auto bomb = make_shared<SFAsset>(SFASSET_BOMB, sf_window);
    auto pos   = Point2((canvas_w/number_of_bombs)*(i+0.5), 440.0f);
    bomb->SetPosition(pos);
    bombs.push_back(bomb);
if(i == 0 ){
std::cout << "Bomb              = Game Over!" << std::endl;
}
  }
  
//Number of bigCoins to be rendered.
//Print out what they are worth (score) to the terminal.
  const int number_of_bigCoins = 5;
  for(int i=0; i<number_of_bigCoins; i++) {
    auto bigCoin = make_shared<SFAsset>(SFASSET_BIGCOIN, sf_window);
    auto pos   = Point2((canvas_w/number_of_bigCoins)*(i+0.5), 50.0f);
    bigCoin->SetPosition(pos);
    bigCoins.push_back(bigCoin);
if(i == 0 ){
std::cout << "Big (5) Coins     = 5 Points!" << std::endl;
}
  }


//Number of smallCoins to be rendered.
//Print out what they are worth (score) to the terminal.
   const int number_of_smallCoins = 4;
   for(int i=0; i<number_of_smallCoins; i++) {
    auto smallCoin = make_shared<SFAsset>(SFASSET_SMALLCOIN, sf_window);
    auto pos   = Point2((canvas_w/number_of_smallCoins)*(i+0.5), 440.0f);
    smallCoin->SetPosition(pos);
    smallCoins.push_back(smallCoin);
if(i == 0 ){
std::cout << "Small (20) Coins  = 20 Points!" << std::endl;
std::cout << "" << std::endl;
}
  }
} 

SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;

//Moving left + collision detection with walls
//if player collides with wall
//player goes in opposite direction (East)
case SFEVENT_PLAYER_LEFT:
  player->GoWest();
    for(auto w : walls) {
      if(player->CollidesWith(w)) {
         player->GoEast();
      }
    }
break;

//Moving right + collision detection with walls
//if player collides with wall
//player goes in opposite direction (West)
case SFEVENT_PLAYER_RIGHT:
  player->GoEast();
   for(auto w : walls) {
      if(player->CollidesWith(w)) {
         player->GoWest();
      }
    }
break;

//Moving up + collision detection with walls
//if player collides with wall
//player goes in opposite direction (South)
case SFEVENT_PLAYER_UP:
     player->GoNorth();
       for(auto w : walls) {
         if(player->CollidesWith(w)) {
            player->GoSouth();
      }
    }
break;



//Moving right + collision detection with walls
//if player collides with wall
//player goes in opposite direction (North)
case SFEVENT_PLAYER_DOWN:
     player->GoSouth();
       for(auto w : walls) {
         if(player->CollidesWith(w)) {
            player->GoNorth();
      } 
    }
break;

  case SFEVENT_FIRE:
    fire ++;
    FireProjectile();
    break;
  }
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // wrap an SDL_Event with our SFEvent
    SFEvent sfevent((const SDL_Event) event);
    // handle our SFEvent
    OnEvent(sfevent);
  }
}

void SFApp::OnUpdateWorld() {
  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorth();
  }



//Remove projectile when colliding with Wall
  for(auto p : projectiles) {
    for(auto w : walls) {
      if(p->CollidesWith(w)) {
        p->SetNotAlive();
      }
    }
  }


//If counter = 5.
//I print out "DOUBLE POINTS" to the terminal
if(counter == 5) {
   std::cout << "DOUBLE POINTS!" << std::endl;
	counter++;
	}


//This is where I handle the collision of the Alien and the player.
//If the alien collides with the player.
//I then print out a line.
//I then print out "Game Over!".
//I then print out "Total Score: ".
//I then print out a line.
//Delay for 1 million microseconds or 1 second.
//then exit the game.
 for(auto a : aliens) {
    if(a->CollidesWith(player)){
        std::cout << "" << std::endl;
        std::cout << "Game Over!" << std::endl;
	std::cout << "Total Score: " << score << std::endl;
        std::cout << "" << std::endl;
	usleep(1000000);
	is_running = false;
break;
}
}

//This is where I handle the collision of the Alien Boss and the player.
//If the alien Boss collides with the player.
//I then print out a line.
//I then print out "Game Over!".
//I then print out "Total Score: ".
//I then print out a line.
//Delay for 1 million microseconds or 1 second.
//then exit the game.
 for(auto i : alienBosses) {
    if(i->CollidesWith(player)){
        std::cout << "" << std::endl;
	std::cout << "Game Over!" << std::endl;
	std::cout << "Total Score: " << score << std::endl;
        std::cout << "" << std::endl;
        usleep(1000000);
	is_running = false;
             break;
}
}


//This is where I handle the collision with the projectiles and Alien.
//If the Projectile collides with the Alien object, I set both objects to not alive and handle the collision.
//I also Add one to my counter each time a coin is destroyed.
//And check if the counter is bigger or less than 5.
//If its bigger the points Scored are multiplied by 2.
//If not the points Scored remain the same.
//I then print out the score to the terminal
 for(auto p : projectiles) {
    for(auto a : aliens) {
      if(p->CollidesWith(a)) {
        p->HandleCollision();
	p->SetNotAlive();
        a->HandleCollision();
	a->SetNotAlive();
	 counter++;
	 alienCounter++;
	if(counter > 5 ){
	score+=20;
        }
	else if(counter <= 5) {
	score+=10;	
	}       
	std::cout << "Score: " << score << std::endl; 
	}
      }
    }


//This is where I handle the collision with the projectiles and big coins.
//If the Projectile collides with the bigCoin object, I set both objects to not alive and handle the collision.
//I also Add one to my counter each time a coin is destroyed.
//And check if the counter is bigger or less than 5.
//If its bigger the points Scored are multiplied by 2.
//If not the points Scored remain the same.
//I then print out the score to the terminal
for(auto p : projectiles) {
    for(auto q : bigCoins) {
      if(p->CollidesWith(q)) {
        p->HandleCollision();
	p->SetNotAlive();
        q->HandleCollision();
	q->SetNotAlive();
       counter++;	
	if(counter > 5) {
	score+=10;
	}
	else if(counter <= 5) {
	score+=5;
	}
	std::cout << "Score: "  << score << std::endl; 
	}
      }
    }

//This is where I handle the collision with the projectiles and small coins.
//If the Projectile collides with the smallCoin object, I set both objects to not alive and handle the collision.
//I also Add one to my counter each time a coin is destroyed.
//I add one to my coin counter...
//This allows me to control the movement of other objects once that coin counter reaches a certain limit.
//And check if the counter is bigger or less than 5.
//If its bigger the points Scored are multiplied by 2.
//If not the points Scored remain the same.
//I then print out the score to the terminal
 for(auto p : projectiles) {
    for(auto r : smallCoins) {
      if(p->CollidesWith(r)) {
        p->HandleCollision();
	p->SetNotAlive();
        r->HandleCollision();
	r->SetNotAlive();
 	counter++;
	coinCounter++;	
        if(counter > 5) {
	score+=40;
	}
	else if(counter <= 5 ){
	score+=20;
	}
	std::cout << "Score: "  << score << std::endl;  
	}
      }
    }

//This is where I handle the collision with the projectiles and Bombs.
//If the Projectile collides with the Bomb object, I set both objects to not alive and handle collision
//I then print out a line.
//I then print out "Game Over!".
//I then print out "Total Score: ".
//I then print out a line.
//Delay for 1million microseconds or 1 second.
//then exit the game.
 for(auto p : projectiles) {
    for(auto b : bombs) {
      if(p->CollidesWith(b)) {
	p->HandleCollision();
	p->SetNotAlive();
        b->HandleCollision();
	b->SetNotAlive();
        std::cout << "" << std::endl;
	std::cout << "Game Over!" << std::endl;
	std::cout << "Total Score: " << score << std::endl;
	std::cout << "" << std::endl;
	usleep(1000000);
	is_running = false;
        break;
    } 

	}
      }

//This is where I handle the collision with the projectiles and Alien Boss.
//If the Projectile collides with the Alien Boss object, I decrement the health by one.
//I then print out the score.
//I check if the health equals zero and if it does I set the object to not being alive
//Add 50 to the score.
//Delay for 1million microseconds or 1 second.
//Print out to the terminal.
//then exit the game.
 for(auto p : projectiles) {
    for(auto i : alienBosses) {
      if(p->CollidesWith(i)) {
       	p->SetNotAlive();
 	p->HandleCollision();
        health1--;
	score+=30;
        std::cout << "Score: " << score << std::endl; 

 if(health1 == 0 ){
	i->SetNotAlive();
	i->HandleCollision();
        score+=50;
        usleep(1000000);

std::cout << "" << std::endl;
std::cout << "You Win!" << std::endl;
std::cout << "Total Score: " << score << std::endl;
std::cout << "" << std::endl;
is_running = false;
break;
	}
      }
    }
}

 
//If the counter in implemented = 16.
//So all the game objects are dead.
//I remove one wall and the bombs for the boss level to start.
for( auto b : bombs){
for( auto s : walls2) {
if( counter >=16){
   b->SetNotAlive(); 
   s->SetNotAlive();
}
}
}

//This starts with my aliens going south.
//If the alien collides with the wall I add one to my alien counter.
//I add one to my print1.
//Check if print1 = 1 then (This causes the next line of code to print only ONCE to the terminal)
//I then print out a line.
//I then print out "Game Over!".
//I then print out "Total Score: ".
//I then print out a line.
//Delay for 1 million microseconds or 1 second.
//then exit the game.
for(auto a : aliens ){
a->GoSouthAlien();
for(auto w : walls ) {
if(a->CollidesWith(w)){
alienCounter++;
print1++;
if(print1 == 1 ){
std::cout << "" << std::endl;
std::cout << "Game Over!" << std::endl;
std::cout << "Total Score: " << score << std::endl;
std::cout << "" << std::endl;
usleep(1000000);
is_running = false;
             break;
}
}
}
}


//This starts with checking if my alien counter = 6.
//This means if all the aliens are dead.
//The small coins start going south.
//If the small coins collide with the wall
//I add 1 to print1
//Check if print1 = 1 then (This causes the next line of code to print only ONCE to the terminal)
//I then print out a line.
//I then print out "Game Over!".
//I then print out "Total Score: ".
//I then print out a line.
//Delay for 1 million microseconds or 1 second.
//then exit the game.
if(alienCounter == 6) {
for(auto r : smallCoins ){
r->GoSouthCoin();
for(auto w : walls ) {
if(r->CollidesWith(w)){
print1++;
if(print1 == 1 ){
std::cout << "" << std::endl;
std::cout << "Game Over!" << std::endl;
std::cout << "Total Score: " << score << std::endl;
std::cout << "" << std::endl;
usleep(1000000);
is_running = false;
             break;
}
}
}
}
}

//This starts with checking if my coin counter = 6.
//This means if all the small coins are dead.
//The big coins start going south.
//If the big coins collide with the wall
//I add 1 to print1
//Check if print1 = 1 then (This causes the next line of code to print only ONCE to the terminal)
//I then print out a line.
//I then print out "Game Over!".
//I then print out "Total Score: ".
//I then print out a line.
//Delay for 1 million microseconds or 1 second.
//then exit the game.
if(coinCounter == 4) {
for(auto q : bigCoins ){
q->GoNorthCoin();
for(auto s : walls2 ) {
if(q->CollidesWith(s)){
print1++;
if(print1 == 1 ){
std::cout << "" << std::endl;
std::cout << "Game Over!" << std::endl;
std::cout << "Total Score: " << score << std::endl;
std::cout << "" << std::endl;
usleep(1000000);
is_running = false;
             break;
}
}
}
}
}

//Handles the collision between my projectiles and the new wall I implemented
for(auto p : projectiles){
for(auto s : walls2) {
    if(p->CollidesWith(s)){
        p->HandleCollision();
	p->SetNotAlive();
  }
}
}

    
// remove dead aliens (the long way)
// remove dead Projectiles (the long way)
// remove dead alien Boss (the long way)
// remove dead Big Coins(the long way)
// remove dead Bombs (the long way)
// remove dead Small Coins (the long way)
// remove dead alien Boss (the long way)
  list<shared_ptr<SFAsset>> tmp;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp.push_back(a);
    }
  }
  aliens.clear();
  aliens = list<shared_ptr<SFAsset>>(tmp);
 

list<shared_ptr<SFAsset>> tmp1;
  for(auto p : projectiles) {
    if(p->IsAlive()) {
      tmp1.push_back(p);
    
  }
}
  projectiles.clear();
  projectiles = list<shared_ptr<SFAsset>>(tmp1);


list<shared_ptr<SFAsset>> tmp2;
  for(auto q : bigCoins) {
    if(q->IsAlive()) {
      tmp2.push_back(q);
    }
  }
  bigCoins.clear();
  bigCoins = list<shared_ptr<SFAsset>>(tmp2);


list<shared_ptr<SFAsset>> tmp3;
  for(auto r : smallCoins) {
    if(r->IsAlive()) {
      tmp3.push_back(r);
    }
  }

  smallCoins.clear();
  smallCoins = list<shared_ptr<SFAsset>>(tmp3);


list<shared_ptr<SFAsset>> tmp4;
  for(auto b : bombs) {
    if(b->IsAlive()) {
      tmp4.push_back(b);
    }
  }
  bombs.clear();
  bombs = list<shared_ptr<SFAsset>>(tmp4);

list<shared_ptr<SFAsset>> tmp5;
  for(auto g : projectiles2) {
    if(g->IsAlive()) {
      tmp5.push_back(g);    
  }
}
  projectiles2.clear();
  projectiles2 = list<shared_ptr<SFAsset>>(tmp5);

  list<shared_ptr<SFAsset>> tmp6;
  for(auto i : alienBosses) {
    if(i->IsAlive()) {
      tmp6.push_back(i);
    }
  }
  alienBosses.clear();
  alienBosses = list<shared_ptr<SFAsset>>(tmp6);


}
void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

//Draw the player
  player->OnRender();
  
//Draw the projectiles 
//Check if they are alive
//If true render
   for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();} 
}

//Draw the projectile2
//Check if they are alive
//If true render
   for(auto p: projectiles2) {
    if(p->IsAlive()) {p->OnRender();}
}

//Draw the aliens 
//Check if they are alive
//If true render
   for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
}
   
//Draw the projectiles 
//Check if they are alive
//If true render
//If counter bigger or equal to 16 (all objects dead apart from alien boss)
//Boss moves south
//If boss collides with wall
//I then print out a line.
//I then print out "Game Over!".
//I then print out "Total Score: ".
//I then print out a line.
//Delay for 1 million microseconds or 1 second.
//then exit the game.
for(auto i: alienBosses) {
if(i->IsAlive()) {i->OnRender();}
if(counter >= 16 ){
for(auto i : alienBosses ){
i->GoSouthAlienBoss();
for(auto w : walls ) {
if(i->CollidesWith(w)){
std::cout << "" << std::endl;
std::cout << "Game Over!" << std::endl;
std::cout << "Total Score: " << score << std::endl;
std::cout << "" << std::endl;
usleep(1000000);
is_running = false;
             break;
}
}
}
 }
}

//Draw the walls 
//Check if they are alive
//If true render
 for(auto w : walls) {
    if(w->IsAlive()) {w->OnRender();}
  }

//Draw the walls2
//Check if they are alive
//If true render
  for(auto s : walls2) {
    if(s->IsAlive()) {s->OnRender();}
  }

//Draw the bombs 
//Check if they are alive
//If true render
 for(auto b : bombs) {
    if(b->IsAlive()) {b->OnRender();}
  } 

//Draw the bigCoins 
//Check if they are alive
//If true render
 for(auto q : bigCoins) {
   if(q->IsAlive()) {q->OnRender();}
  }

//Draw the smallCoins
//Check if they are alive
//If true render
 for(auto r : smallCoins) {
   if(r->IsAlive()) {r->OnRender();}
  }


// Switch the off-screen buffer to be on-screen
//If counter is bigger or equal to five we change the projectile sprite.
  SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::FireProjectile() {
  if(counter < 5) {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}
else if(counter >= 5) {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE2, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}
}


#include "SFApp.h"
#include <unistd.h>


SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), score(0), counter(0), alienCounter(0), health1(15), coinCounter(0), is_running(true), sf_window(window) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2,100);
  player->SetPosition(player_pos);


 const int number_of_alienBosses = 1;
 for(int i=0; i<number_of_alienBosses; i++) {
 auto alienBoss  = make_shared<SFAsset>(SFASSET_ALIENBOSS, sf_window);
  auto alienBoss_pos = Point2((canvas_w/number_of_alienBosses) * (i+0.5), 530.0f);
  alienBoss->SetPosition(alienBoss_pos);
  alienBosses.push_back(alienBoss);
 }

  const int number_of_aliens = 6;
  for(int i=0; i<number_of_aliens; i++) {
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
    auto pos   = Point2((canvas_w/number_of_aliens) * (i+0.5), 320.0f);
    alien->SetPosition(pos);
    aliens.push_back(alien);
  }

const int number_of_wallsx = 20;
  for(int i=0; i<number_of_wallsx; i++) {
    // place an alien at width/number_of_aliens * i
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2((canvas_w/number_of_wallsx) * i, 0.0f);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }


const int number_of_wallsx2 = 20;
  for(int i=0; i<number_of_wallsx2; i++) {
    // place an alien at width/number_of_aliens * i
    auto wall = make_shared<SFAsset>(SFASSET_WALL2, sf_window);
    auto pos   = Point2((canvas_w/number_of_wallsx2) * i, 480.0f);
    wall->SetPosition(pos);
    walls2.push_back(wall);
  }


const int number_of_wallsy = 20;
  for(int i=0; i<number_of_wallsy; i++) {
    // place an alien at width/number_of_aliens * i
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2(0.0f, (canvas_w/number_of_wallsy) * i);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }


const int number_of_wallsy2 = 20;
  for(int i=0; i<number_of_wallsy2; i++) {
    // place an alien at width/number_of_aliens * i
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2(640.0f, (canvas_w/number_of_wallsy2) * i);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }
  
const int number_of_wallsHorizontal = 5;
  for(int i=0; i<number_of_wallsHorizontal; i++) {
    auto wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
    auto pos   = Point2((canvas_w/number_of_wallsHorizontal) * i, 120.0f);
    wall->SetPosition(pos);
    walls.push_back(wall);
  }
  

const int number_of_bombs = 3;
  for(int i=0; i<number_of_bombs; i++) {
    auto bomb = make_shared<SFAsset>(SFASSET_BOMB, sf_window);
    auto pos   = Point2((canvas_w/number_of_bombs)*(i+0.5), 440.0f);
    bomb->SetPosition(pos);
    bombs.push_back(bomb);
  }
  
  
  const int number_of_bigCoins = 5;
  for(int i=0; i<number_of_bigCoins; i++) {
    auto bigCoin = make_shared<SFAsset>(SFASSET_BIGCOIN, sf_window);
    auto pos   = Point2((canvas_w/number_of_bigCoins)*(i+0.5), 50.0f);
    bigCoin->SetPosition(pos);
    bigCoins.push_back(bigCoin);
  }



   const int number_of_smallCoins = 4;
   for(int i=0; i<number_of_smallCoins; i++) {
    auto smallCoin = make_shared<SFAsset>(SFASSET_SMALLCOIN, sf_window);
    auto pos   = Point2((canvas_w/number_of_smallCoins)*(i+0.5), 440.0f);
    smallCoin->SetPosition(pos);
    smallCoins.push_back(smallCoin);
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
case SFEVENT_PLAYER_LEFT:
  player->GoWest();
    for(auto w : walls) {
      if(player->CollidesWith(w)) {
         player->GoEast();
      }
    }
break;

//Moving right + collision detection with walls
case SFEVENT_PLAYER_RIGHT:
  player->GoEast();
   for(auto w : walls) {
      if(player->CollidesWith(w)) {
         player->GoWest();
      }
    }
break;

//Moving up + collision detection with walls
case SFEVENT_PLAYER_UP:
     player->GoNorth();
       for(auto w : walls) {
         if(player->CollidesWith(w)) {
            player->GoSouth();
      }
    }
break;



//Moving right + collision detection with walls
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

if(counter == 5) {
   std::cout << "DOUBLE POINTS!" << std::endl;
	counter++;
	}


// Detect collisions (alien - projectile)
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

// Detect collisions (alien - projectile)
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
std::cout << "Total Score: " << score << std::endl;
std::cout << "Game Over" << std::endl;

	}
      }
    }
}
 
    for(auto a : aliens) {
	if(a->CollidesWith(player)){
	std::cout << "Total Score: " << score << std::endl;
        std::cout << "Game Over" << std::endl;
	is_running = false;
             break;
}
}

     for(auto i : alienBosses) {
	if(i->CollidesWith(player)){
	std::cout << "Total Score: " << score << std::endl;
        std::cout << "Game Over" << std::endl;
	is_running = false;
             break;
}
}
// Detect collisions (alien - projectile)
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


for( auto b : bombs){
for( auto s : walls2) {
if( counter >=16){
   b->SetNotAlive(); 
   s->SetNotAlive();
}
}
}


for(auto a : aliens ){
a->GoSouthAlien();
for(auto w : walls ) {
if(a->CollidesWith(w)){
alienCounter++;
std::cout << "Game Over" << std::endl;
std::cout << "Total Score: " << score << std::endl;
 usleep(40000);
is_running = false;
             break;
}
}
}

if(alienCounter == 6) {
for(auto r : smallCoins ){
r->GoSouthCoin();
for(auto w : walls ) {
if(r->CollidesWith(w)){
std::cout << "Game Over" << std::endl;
std::cout << "Total Score: " << score << std::endl;
is_running = false;
             break;
}
}
}
}

if(coinCounter == 4) {
for(auto q : bigCoins ){
q->GoNorthCoin();
for(auto s : walls2 ) {
if(q->CollidesWith(s)){
std::cout << "Game Over" << std::endl;
std::cout << "Total Score: " << score << std::endl;
is_running = false;
             break;
}
}
}
}

for(auto p : projectiles){
for(auto s : walls2) {
    if(p->CollidesWith(s)){
        p->HandleCollision();
	p->SetNotAlive();
  }
}
}


 for(auto p : projectiles) {
    for(auto b : bombs) {
      if(p->CollidesWith(b)) {
	p->HandleCollision();
	p->SetNotAlive();
        b->HandleCollision();
	b->SetNotAlive();
	std::cout << "Game Over" << std::endl;
	std::cout << "Total Score: " << score << std::endl;
	is_running = false;
        break;
    } 

	}
      }
    
// remove dead aliens (the long way)
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

  // draw the player
  player->OnRender();
  

   for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();} 
}

   for(auto p: projectiles2) {
    if(p->IsAlive()) {p->OnRender();}
}


   for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
}
   

for(auto i: alienBosses) {
if(i->IsAlive()) {i->OnRender();}
if(counter >= 16 ){
for(auto i : alienBosses ){
i->GoSouthAlien();
for(auto w : walls ) {
if(i->CollidesWith(w)){
std::cout << "Game Over" << std::endl;
std::cout << "Total Score: " << score << std::endl;
is_running = false;
             break;
}
}
}
 }
}


 for(auto w : walls) {
    if(w->IsAlive()) {w->OnRender();}
  }

  for(auto s : walls2) {
    if(s->IsAlive()) {s->OnRender();}
  }

 for(auto b : bombs) {
    if(b->IsAlive()) {b->OnRender();}
  } 


 for(auto q : bigCoins) {
   if(q->IsAlive()) {q->OnRender();}
  }

 for(auto r : smallCoins) {
   if(r->IsAlive()) {r->OnRender();}
  }


  // Switch the off-screen buffer to be on-screen
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


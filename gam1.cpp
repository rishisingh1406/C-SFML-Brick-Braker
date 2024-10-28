// birck breaker game made in C++ 

# include <iostream> 
# include <SFML/Graphics.hpp>
# include <vector>
# include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

// game variables 

int screen_height = 800;
int screen_width = 646;
int paddle_width = 120;
int paddle_height = 30;
Color paddle_color(3, 244, 252, 255);
int paddle_x = 150;
int paddle_y = 700;
int paddle_speed = 15;
int brick_width = 60;
int brick_height = 30;
int ball_radious = 10;
int ball_x = 130;
int ball_y = 600;
float ball_speed_x = 4;
float ball_speed_y = 4;
vector<RectangleShape> brickList;
int FPS = 60;
int score = 0; 
int score_x = 0; 
int score_y = 750; 
bool isGameOver = false;

// colors for bricks 
Color colorArray_brick[10] = {

	Color(255, 80, 80),
	Color(102, 255, 51),      // Green
	Color(0, 153, 255),    // Blue
	Color(255, 255, 0, 255),  // Yellow
	Color(0, 255, 255, 255),  // Cyan
	Color(255, 165, 0, 255),  // Orange
	Color(128, 0, 128, 255),  // Purple
	Color(255, 105, 180, 255),// Hot Pink
	Color(64, 224, 208, 255), // Turquoise
	Color(255, 192, 203, 255) // Pink
};


// creating the game paddle 
class game_paddle : public RectangleShape {
public:

	game_paddle() {
		setSize(Vector2f(paddle_width, paddle_height));
		setFillColor(paddle_color);
		setPosition(paddle_x, paddle_y);

	}

	// for updating the game paddle location 
	void updatePosition(int x) {

		paddle_x += x;
		setPosition(paddle_x, paddle_y);

	}
};

// creating the brick 
class brick : public RectangleShape {
public:

	brick(int locx, int locy, Color color) {

		setSize(Vector2f(brick_width, brick_height));
		setFillColor(color);
		setPosition(locx, locy);

	}

};

// crating multiple brick 

void create_brick() {
	for (int i = 0; i < 10; i++)
	{
		int y_loc = i * (brick_height + 5);

		for (int x = 0; x < 10;x++) {
			int x_loc = x * (brick_width + 5);
			brick bricks(x_loc, y_loc, colorArray_brick[i]);
			brickList.push_back(bricks);


		}
	}
}


// creating the ball and its property 

class balls : public CircleShape {
public:

	balls(int ball_radious, float x, float y) : CircleShape(ball_radious) {
		setPosition(x, y);
		setFillColor(Color(255, 255, 255));

	}

	void ball_update_position(float x, float y) {
		ball_x += x;
		ball_y += y;
		setPosition(ball_x, ball_y);
	}

	// code for checking boundary collision  
	void check_boundary_colision() {
		if (getPosition().x <= 0 || getPosition().x + 2 * ball_radious >= screen_width) {
			ball_speed_x = -ball_speed_x; 
		}
		if (getPosition().y <= 0) {
			ball_speed_y = -ball_speed_y; 
		}
		if (getPosition().y > screen_height) {
			isGameOver = true; 
		}
	}
};


//  checking the collision  for entire game 

class collosion {
public:
	SoundBuffer buffer;
	Sound sound;

	// collision with the paddle 
	void check_paddle_collosion() {


		if (ball_y + ball_radious * 2 >= paddle_y &&
			ball_x + ball_radious >= paddle_x &&
			ball_x <= paddle_x + paddle_width)
			ball_speed_y = -ball_speed_y;
		
	}
	// collision with the bricks 
	void check_collision_brick(balls &ball , vector<RectangleShape>& bricks) {

		for (auto& brick : bricks)
		{
			bool brickHit = false;
			if (brick.getGlobalBounds().intersects(ball.getGlobalBounds()) &&!brickHit) {
				ball_speed_y = -ball_speed_y;
				score += 1;
				brick.setPosition(-100, -100); // Move brick off-screen when hit
				
				// code for sound property 

                
                if (!buffer.loadFromFile("brick_break_sound.wav")) {
	              // Handle error
                       }
				
				sound.setBuffer(buffer);
				sound.play();


				brickHit = true;
			}

		}


	}

};

// code  for creating  score 

class scores : public Font, public Text {
public:
	Font font;
	Text text;
	int score;

	// Constructor to initialize the text properties

	scores(int initialScore) : score(initialScore) {
		// Load a font file
		if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
			// Handle error
			cerr << "Error loading font" << endl;

		}

		text.setFont(font);                         
		text.setString("Score: " + to_string(score)); 
		text.setCharacterSize(30);                  
		text.setFillColor(Color::White);            
		text.setPosition(score_x, score_y);                   
	}

	// Function to update score
	void updateScore(int newScore) {
		score = newScore;
		text.setString("Score: " + to_string(score));
	}
};


// code for creating game over 
class gameover : public Font , public Text{
public: 
	Font font; 
	Text text; 
	gameover() {

		if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
			// Handle error
			cerr << "Error loading font" << endl;

		}

		text.setFont(font);
		text.setString("Game Over");
		text.setCharacterSize(100);
		text.setFillColor(Color::White);
		text.setPosition(75,400);
	


	}



};

// Managing the basic screen function 

void create_screen() {
	RenderWindow screen(VideoMode(screen_width, screen_height), "Brick Breaker", Style::Titlebar | Style::Close);
	Event ev;
	game_paddle paddle;
	collosion colod;
	scores scoreb(score); 
	gameover over; 

	balls ball(ball_radious, ball_x, ball_y);


	create_brick();

	while (screen.isOpen()) {

		while (screen.pollEvent(ev)) {

			// managing the screen event 

			switch (ev.type) {

			case Event::Closed:
				screen.close();
				break;

				// managing the paddle movement 

			case Event::KeyPressed:
				if (ev.key.code == Keyboard::Left) {
					if (paddle_x > 0)
						paddle.updatePosition(-paddle_speed);
				}

				else if (ev.key.code == Keyboard::Right) {
					if (paddle_x < 530)
						paddle.updatePosition(paddle_speed);
				}

				break;
			}




		}
		if (!isGameOver) {
			ball.check_boundary_colision();
			colod.check_paddle_collosion();
			colod.check_collision_brick(ball, brickList);
			scoreb.updateScore(score);
			ball.ball_update_position(ball_speed_x, ball_speed_y);
		}

		screen.clear(Color::Black);
		screen.draw(paddle);
		screen.draw(ball);
		screen.draw(over);
		screen.draw(scoreb.text);
		// game over txt 
		if (isGameOver) {
			screen.draw(over.text);
		}
		// drawing the bricklist 
		for (auto& b : brickList) {
			screen.draw(b);
		}
		screen.setFramerateLimit(FPS);

		screen.display();
	}
}


int main() {
	create_screen();
	return 0;
}
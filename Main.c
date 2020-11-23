#include <stdio.h>
#include <SDL.h>

#include "Constants.h"

int game_is_running;
int last_frame_time;
int direction_check_x;
int direction_check_y;
int player_one_score;
int player_two_score;
float delta_time;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

struct object {
	float x;
	float y;
	float width;
	float height;
};
struct object ball;
struct object player_one;
struct object player_two;
struct object net;

int initialize_window() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return FALSE;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return FALSE;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return FALSE;
	}

	return TRUE;
}

void setup() {
	ball.x = (WINDOW_WIDTH / 2) - (BALL_WIDTH / 2);
	ball.y = (WINDOW_HEIGHT / 2) - (BALL_HEIGHT / 2);
	ball.width = BALL_WIDTH;
	ball.height = BALL_HEIGHT;

	player_one.x = 20;
	player_one.y = (WINDOW_HEIGHT / 2) - (PLAYER_HEIGHT / 2);
	player_one.width = PLAYER_WIDTH;
	player_one.height = PLAYER_HEIGHT;

	player_two.x = 760;
	player_two.y = (WINDOW_HEIGHT / 2) - (PLAYER_HEIGHT / 2);
	player_two.width = PLAYER_WIDTH;
	player_two.height = PLAYER_HEIGHT;

	net.x = WINDOW_WIDTH / 2;
	net.y = 0;
	net.width = NET_WIDTH;
	net.height = NET_HEIGHT;
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			game_is_running = FALSE;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				game_is_running = FALSE;
			}
			else if (event.key.keysym.sym == SDLK_DOWN && player_two.y < WINDOW_HEIGHT - PLAYER_HEIGHT) {
				player_two.y += PLAYER_SPEED * delta_time;
			}
			else if (event.key.keysym.sym == SDLK_UP && player_two.y > 0) {
				player_two.y -= PLAYER_SPEED * delta_time;
			}
			else if (event.key.keysym.sym == SDLK_s && player_one.y < WINDOW_HEIGHT - PLAYER_HEIGHT) {
				player_one.y += PLAYER_SPEED * delta_time;
			}
			else if (event.key.keysym.sym == SDLK_w && player_one.y > 0) {
				player_one.y -= PLAYER_SPEED * delta_time;
			}
			break;
	}
}

void center_ball() {
	ball.x = (WINDOW_WIDTH / 2) - (BALL_WIDTH / 2);
	ball.y = (WINDOW_HEIGHT / 2) - (BALL_HEIGHT / 2);
	//SDL_Delay(1000);
}

void update() {
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();

	//CHECKING FOR CORNER HITS
	if (ball.x < 0) {
		center_ball();
		player_two_score++;
		direction_check_x = 0;
	}
	else if (ball.x > WINDOW_WIDTH - BALL_WIDTH) {
		center_ball();
		player_one_score++;
		direction_check_x = 1;
	}

	if (ball.y < 0) {
		direction_check_y = 0;
	}
	else if (ball.y > WINDOW_HEIGHT - BALL_HEIGHT) {
		direction_check_y = 1;
	}

	//CHECKING FOR PLAYER HITS
	if (ball.x < player_one.x + PLAYER_WIDTH && (ball.y + BALL_HEIGHT >= player_one.y && ball.y <= player_one.y + PLAYER_HEIGHT)) {
		direction_check_x = 0;
	}

	if (ball.x + BALL_WIDTH > player_two.x && (ball.y + BALL_HEIGHT >= player_two.y && ball.y <= player_two.y + PLAYER_HEIGHT)) {
		direction_check_x = 1;
	}

	//CHECKING FOR BALL DIRECTION
	if (direction_check_x) {
		ball.x -= BALL_SPEED * delta_time;
	}
	else {
		ball.x += BALL_SPEED * delta_time;
	}

	if (direction_check_y) {
		ball.y -= BALL_SPEED * delta_time;
	}
	else {
		ball.y += BALL_SPEED * delta_time;
	}

	//ENDING THE GAME
	if (player_one_score == 5 || player_two_score == 5) {
		game_is_running = FALSE;
	}
}

void render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	printf("BALL_X: %.2f, BALL_Y %.2f\n", ball.x, ball.y);
	printf("PLAYER_ONE_X: %.2f, PLAYER_ONE_Y: %.2f\n", player_one.x, player_one.y);
	printf("PLAYER_TWO_X: %.2f, PLAYER_TWO_Y: %.2f\n", player_two.x, player_two.y);

	SDL_Rect ball_rect = {
		ball.x,
		ball.y,
		ball.width,
		ball.height
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_RenderFillRect(renderer, &ball_rect);

	SDL_Rect player_one_rect = {
		player_one.x,
		player_one.y,
		player_one.width,
		player_one.height
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_RenderFillRect(renderer, &player_one_rect);

	int score_position_player_one = 20 + BALL_WIDTH * 2;
	for (int i = 0; i < player_one_score; i++) {
		SDL_Rect player_one_score_rect = {
			score_position_player_one,
			20,
			ball.width,
			ball.height
		};

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderFillRect(renderer, &player_one_score_rect);

		score_position_player_one += BALL_WIDTH * 2;
	}

	SDL_Rect player_two_rect = {
		player_two.x,
		player_two.y,
		player_two.width,
		player_two.height
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_RenderFillRect(renderer, &player_two_rect);

	int score_position_player_two = 760 - BALL_WIDTH * 2;
	for (int i = 0; i < player_two_score; i++) {
		SDL_Rect player_two_score_rect = {
			score_position_player_two,
			20,
			ball.width,
			ball.height
		};

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderFillRect(renderer, &player_two_score_rect);

		score_position_player_two -= BALL_WIDTH * 2;
	}

	for (int i = 0; i < WINDOW_HEIGHT - NET_HEIGHT; i += NET_HEIGHT * 2) {
		net.y = i;

		SDL_Rect net_rect = {
			net.x,
			net.y,
			net.width,
			net.height
		};

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderFillRect(renderer, &net_rect);		
	}

	SDL_RenderPresent(renderer);
}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* args[]) {
	game_is_running = initialize_window();

	setup();

	while (game_is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

//Default camera movements
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	//camera attributes
	glm::vec3 cam_pos;
	glm::vec3 cam_front_v;
	glm::vec3 cam_right_v;
	glm::vec3 cam_up_v;
	glm::vec3 world_up_v;

	//euler angles
	float yaw;
	float pitch;

	//camera options
	float mouse_sensitivity;
	float movement_speed;
	float zoom;

	//constructor with vectors
	Camera(glm::vec3 cam_pos = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 world_up_v = glm::vec3(0.0f,1.0f,0.0f),float yaw = YAW, float pitch = PITCH ) : cam_pos(cam_pos), world_up_v(world_up_v),yaw(yaw), pitch(pitch), cam_front_v(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
		update_camera_vectors();
	}

	//constructor with scalers
	Camera(float pos_x, float pos_y, float pos_z, float world_up_x, float world_up_y, float world_up_z, float yaw, float pitch) {
		cam_pos = glm::vec3(pos_x, pos_y, pos_z);
		world_up_v = glm::vec3(world_up_x, world_up_y, world_up_z);
		this->yaw = yaw;
		this->pitch = pitch;

		update_camera_vectors();
	}
	
	//return the view matrix
	glm::mat4 get_view_mat() {
		return glm::lookAt(cam_pos, cam_pos + cam_front_v, world_up_v);
	}

	//process key press
	void process_keyboard(Camera_Movement direction, float dt) {
		float velocity = movement_speed * dt;
		
		if (direction == FORWARD)  cam_pos += cam_front_v * velocity;
		if (direction == BACKWARD) cam_pos -= cam_front_v * velocity;
		if(direction == RIGHT) cam_pos += cam_right_v * velocity;
		if(direction == LEFT) cam_pos -= cam_right_v * velocity;
		if (direction == UP) cam_pos += cam_up_v * velocity;
		if (direction == DOWN) cam_pos -= cam_up_v * velocity;
		
	}

	//process mouse movements
	void process_mouse_move(float x_offset, float y_offset, GLboolean constraint_pitch = true) {
		yaw += x_offset * mouse_sensitivity;
		pitch += y_offset * mouse_sensitivity;

		if (constraint_pitch) {
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			if (pitch < -89.0f) {
				pitch = -89.0f;
			}
		}

		update_camera_vectors();
	}

	//process mouse scroll
	void process_mouse_scroll(float y_offset) {
		zoom -= (float)y_offset;	
		if (zoom < 1.0f) zoom = 1.0f;
		if (zoom > 45.0f) zoom = 45.0f;
	}

private:
	void update_camera_vectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cam_front_v = glm::normalize(front);

		cam_right_v = glm::normalize(glm::cross(cam_front_v, world_up_v));
		cam_up_v = glm::normalize(glm::cross(cam_right_v,cam_front_v));
		
	}

};
#endif
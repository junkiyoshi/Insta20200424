#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofEnableDepthTest();

	this->base_radius = 50;
	this->number_of_satellite = 15;
	for (int k = 0; k < this->number_of_satellite; k++) {

		this->satellite_location_list.push_back(glm::normalize(glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1))) * this->base_radius * 1.2);
		this->satellite_noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));

	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->number_of_satellite; i++) {

		auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(this->satellite_noise_seed_list[i].x, (ofGetFrameNum() + 0) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(0, 1, 0));
		auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(this->satellite_noise_seed_list[i].y, (ofGetFrameNum() + 0) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(1, 0, 0));
		auto satellite_location = glm::vec4(this->satellite_location_list[i], 0) * rotation_y * rotation_x;

		auto next_rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(this->satellite_noise_seed_list[i].x, (ofGetFrameNum() + 1) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(0, 1, 0));
		auto next_rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(this->satellite_noise_seed_list[i].y, (ofGetFrameNum() + 1) * 0.005), 0, 1, -PI * 2, PI * 2), glm::vec3(1, 0, 0));
		auto next_satellite_location = glm::vec4(this->satellite_location_list[i], 0) * next_rotation_y * next_rotation_x;

		auto distance = satellite_location - next_satellite_location;

		this->frame.addVertex(satellite_location);
		this->frame.addColor(ofColor(39));
		this->velocity_list.push_back(distance * 0.75);
	}

	for (int i = 0; i < this->frame.getNumVertices(); i++) {

		this->frame.setVertex(i, this->frame.getVertex(i) + this->velocity_list[i]);
	}

	for (int i = this->velocity_list.size() - 1; i >= 0; i--) {

		if (glm::length(this->frame.getVertex(i)) > 150) {

			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->frame.removeVertex(i);
		}
	}

	this->frame.clearIndices();
	for (int i = 0; i < this->frame.getNumVertices(); i++) {

		this->frame.setColor(i, ofColor(239, 39, 39, ofMap(glm::length(this->frame.getVertex(i)), 50, 150, 255, 0)));
		for (int k = i + 1; k < this->frame.getNumVertices(); k += 1) {

			if (glm::distance(this->frame.getVertex(i), this->frame.getVertex(k)) < 20) {

				this->frame.addIndex(i); this->frame.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum());
	ofRotateX(ofGetFrameNum() * 0.75);

	ofSetColor(239, 39, 39);
	ofDrawSphere(glm::vec3(), 30);

	for (int i = 0; i < this->frame.getNumVertices(); i++) {

		ofSetColor(this->frame.getColor(i));
		ofDrawSphere(this->frame.getVertex(i), 1);
	}

	this->frame.drawWireframe();


	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
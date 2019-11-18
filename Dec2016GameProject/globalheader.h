#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<Box2D\Box2D.h>
#include<math.h>
#include<iostream>
#include<vector>
#include<set>
#include"GameHeader.h"

#ifndef VECTOR2D
#define VECTOR2D
struct Vector2D {
	float x;
	float y;
	Vector2D(Vector2D*u) {
		x = u->x;
		y = u->y;
	}
	Vector2D(float x = 0, float y = 0)
		:x(x), y(y) {}
	~Vector2D(){}
	Vector2D* get() {
		return new Vector2D(x, y);
	}
	void set(Vector2D u) 
	{
		x = u.x;
		y = u.y;
	}
	void set(Vector2D*u) {
		x = u->x;
		y = u->y;
	}

	void set(float x, float y) {
		this->x = x;
		this->y = y;
	}
	
	void add(Vector2D otherVector2D) {
		this->x += (otherVector2D.x);
		this->y += (otherVector2D.y);
	}
	void add(Vector2D*otherVector2D) {
		this->x += (otherVector2D->x);
		this->y += (otherVector2D->y);
	}
	void add(float x, float y) {
		this->x += x;
		this->y += y;
	}
	
	void subtract(Vector2D v) {
		x -= v.x;
		y -= v.y;
	}
	void subtract(Vector2D*v) {
		x -= v->x;
		y -= v->y;
	}
	void subtract(float x, float y) {
		this->x -= x;
		this->y -= y;
	}
	void multiply(float d) {
		x *= d;
		y *= d;
	}
	void divide(float d) {
		x /= d;
		y /= d;
	}

	float magnitude() {
		return sqrtf(x*x + y*y);
	}
	void setMagnitude(float newMag) {
		normalize();
		multiply(newMag);
	}

	void normalize() {
		float m = magnitude();
		if (m != 0)
			divide(m);
	}
	void limit(float max) {
		if (magnitude() > max) {
			normalize();
			setMagnitude(max);
		}
	}
	float getAngle() {
		return atan2f(y,x);
	}
	//static methods////////////////
	static Vector2D*add(Vector2D*u, Vector2D*v) {
		return new Vector2D(u->x + v->x, u->y + v->y);
	}
	static Vector2D add(Vector2D u,Vector2D v) {
		return Vector2D(u.x + v.x, u.y + v.y);
	}
	static Vector2D*subtract(Vector2D*u, Vector2D*v) {
		return new Vector2D(u->x - v->x, u->y - v->y);
	}
	static Vector2D subtract(Vector2D u,Vector2D v) {
		return Vector2D(u.x - v.x, u.y - v.y);
	}
	static Vector2D*multiply(Vector2D*u, float k) {
		return new Vector2D(u->x*k, u->y*k);
	}
	static Vector2D*divide(Vector2D*u, float k) {
		return Vector2D::multiply(u, 1 / k);
	}

	static Vector2D divide(Vector2D u, float k) {
		return Vector2D(u.x/k,u.y/k);
	}
	static Vector2D*normalize(Vector2D*u) {
		return Vector2D::divide(u, u->magnitude());
	}
	static float scalarMultiply(Vector2D*u, Vector2D*v) {
		return (u->x*v->x + u->y*v->y);
	}
	static float scalarMultiply(Vector2D u,Vector2D v) {
		return (u.x*v.x + u.y*v.y);
	}
	static float distance(Vector2D*u, Vector2D*v) {
		return (sqrtf((u->x - v->x)*(u->x - v->x) + (u->y - v->y)*(u->y - v->y)));
	}
	static float distance(Vector2D u,Vector2D v) {
		return (sqrtf((u.x - v.x)*(u.x - v.x) + (u.y - v.y)*(u.y - v.y)));
	}
	static float distanceSqr(Vector2D u, Vector2D v) {
		return ((u.x - v.x)*(u.x - v.x) + (u.y - v.y)*(u.y - v.y));
	}
	static float distanceSqr(Vector2D*u, Vector2D*v) {
		return ((u->x - v->x)*(u->x - v->x) + (u->y - v->y)*(u->y - v->y));
	}
};
#endif // !VECTOR2D
struct Size {
	Size(float w = 0, float h = 0)
		:w(w), h(h)
	{}
	~Size() {
		w = 0;
		h = 0;
	}
	void set(float w, float h) {
		this->w = w;
		this->h = h;
	}
	float getHalfPerimeter() {
		return w + h;
	}
	float w;
	float h;
};
struct Color {
	Color(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255)
		:r(r), g(g), b(b)
	{}
	void set(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255) {
		this->r = r;
		this->b = b;
		this->g = g;
	}
	Uint8 r;
	Uint8 g;
	Uint8 b;
};

struct Rectangle {
	Rectangle(float32 x = 0, float32 y = 0, float32 w = 0, float32 h = 0)
		:x(x),y(y),w(w),h(h)
	{}
	void set(float32 x = 0, float32 y = 0, float32 w = 0, float32 h = 0) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
	float32 x;
	float32 y;
	float32 w;
	float32 h;
};

#ifndef NODE_H
#define NODE_H
template<class T>
struct Node {
	Node*next;
	Node*prev;
	T data;
	Node(Node*prev, Node*next, T data)
		:prev(prev), next(next), data(data)
	{}
	//there is no destructor because
	//all the pointer is put from out_side
};
#endif // !NODE_H

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template<class T>
struct LinkedList {
	LinkedList() {
		head = new Node<T>(NULL, NULL, (T)0);
		tail = new Node<T>(head, NULL, (T)0);
		head->next = tail;
		numNodes = 0;
	}
	LinkedList(Node<T>*firstNode, Node<T>*lastNode) {
		head = new Node<T>(NULL, firstNode, (T)0);
		tail = new Node<T>(lastNode, NULL, (T)0);
		for (Node<T>*iter = head->next; iter != tail->prev->next; iter = iter->next)
			numNodes++;
	}
	~LinkedList() {
		//what if type of data is a dynamic pointer
		emptyList();
		delete head;
		delete tail;
		head = NULL;
		tail = NULL;
		numNodes = 0;
	}

	void addFirst(T data) {
		addNode(head, data);
	}
	void addLast(T data) {
		addNode(tail->prev, data);
	}
	void delFist() {
		if (head->next != tail)
			delNode(head->next);
	}
	void delLast() {
		if (tail->prev != head)
			delNode(tail->prev);
	}
	void addNode(Node<T>*pointer, T data) {
		Node<T>*newNode = new Node<T>(pointer, pointer->next, data);
		newNode->next->prev = newNode;
		newNode->prev->next = newNode;
		numNodes++;
	}
	bool emptyList() {
		for (Node<T>*iter = head->next; iter != tail; iter = iter->next)
			iter = delNode(iter);
		return true;
	}
	Node<T>* delNode(Node<T>*pointer) {
		Node<T>*temp = pointer->prev;
		pointer->next->prev = pointer->prev;
		pointer->prev->next = pointer->next;
		pointer->next = NULL;
		pointer->prev = NULL;
		delete pointer;
		pointer = NULL;
		numNodes--;
		return temp;
	}
	bool isEmpty()const {
		return(numNodes == 0);
	}
	int getSize()const {
		return this->numNodes;
	}
	Node<T>*getFirstNode() {
		return head->next;
	}
	//two controling nodes
	Node<T>*head;
	Node<T>*tail;
	int numNodes;
};
#endif // !LINKED_LIST_H

#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H
class UtilityFunctions{
public:

	static Vector2D*polarToCartesian(Vector2D&origin, float radius, float angle) {
		float x = radius*cosf(angle);
		float y = radius*sinf(angle);
		Vector2D*newVector = new Vector2D(origin.x+x,origin.y+y);
		return newVector;
	}
	static Vector2D*polarToCartesian(int x0,int y0, float radius, float angle) {
		float x = radius*cosf(angle);
		float y = radius*sinf(angle);
		Vector2D*newVector = new Vector2D(x0+x,y0+y);
		return newVector;
	}
	static float map(float value, float minOfValue, float maxOfValue, float minOfNewValue, float maxOfNewValue) {
		float a = value - minOfValue;
		float rate = a / (maxOfValue - minOfValue);
		float b = rate*(maxOfNewValue - minOfNewValue);
		b += minOfNewValue;
		return b;
	}
	static void drawCircle(SDL_Renderer**renderer,Vector2D&position,float radius) {
		//dynamic declarations
		Vector2D*oldPos = new Vector2D();
		Vector2D*newPos = new Vector2D();

		for (int i = 0; i <= 360; i++) {
			float angle = (float)i*3.141592f / 180.0f;
			
			if (i == 0) {
				delete oldPos;//delete current memory
				//create a new dynamic Vector for the first use of oldPos
				oldPos = new Vector2D(position.x + radius, position.y);
			}
			//create a new Vector so don't forget to detroy the dynamic memory first
			delete newPos;
			//by point to a new location of memory where stores a Vector
			newPos = polarToCartesian(position, radius, angle);
			//draw
			SDL_RenderDrawLine(*renderer, (int)oldPos->x, (int)oldPos->y, (int)newPos->x, (int)newPos->y);
			//delete first
			delete oldPos;
			//copy data from newPos by create a new dynamic memory location and point to that
			oldPos = new Vector2D(newPos);

		}
		//destroy them all
		delete oldPos;
		delete newPos;
	}
	
	//constrain function is to keep the value of x 
	//equated to a if smaller than a
	//b if greater than b
	template<class T>
	static const T& constrain(const T& x, const T& a, const T& b) {
		if (x < a) {
			return a;
		}
		else if (x>b) {
			return b;
		}
		else
			return x;
	}
	template<class T>
	static const T& min(const T&a, const T&b) {
		return (a < b ? a : b);
	}
	template<class T>
	static const T& max(const T&a, const T&b) {
		return (a > b ? a : b);
	}


	//Pixel to SI
	static float Pixel2SIf(float pixels) {
		return (float)(pixels/M2P());
	}
	static float SI2Pixelf(float si) {
		return (float)(si*M2P());
	}

	static float M2P() {
		return 60.0f;
	}
	static float P2M() {
		return 1.0f/M2P();
	}
	static int charToInt(char c) {
		return c - '0';
	}
	
/*	static void rotateVector(b2Vec2*vector, float angle) {
		angle += atan2f(vector->x, vector->y);
		float length = vector->Length();
		vector->Set(length*cosf(angle), length*sinf(angle));
	}
	*/
};
#endif // !UTILITYFUNCTION_H
#ifndef CONSTANTS_H
#define PI 3.141592f
#endif // !CONSTANTS_H

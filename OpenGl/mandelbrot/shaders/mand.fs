#version 410 core

uniform int itr;
uniform double zoom;
uniform dvec2 screenSize;
uniform dvec2 offset;

double n = 0.0;
double threshold = 100.0;

double mandelbrot(dvec2 c){
	dvec2 z = vec2(0.0, 0.0);
	for(int i = 0; i < itr; i++){
		dvec2 znew;
		znew.x = (z.x * z.x) - (z.y * z.y) + c.z;
		znew.y = (2.0 * z.x * z.y) + c.y;
		z = znew;
		if((z.x * z.x) + (z.y * z.y) > threshold)
			break;
		n++;
	}
	return n / float(itr);
}

vec4 mapToColor(float t){
	float r = 9.0 * (1.0 - t) * t * t * t;
	float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
	float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

	return vec4(r, g, b, 1.0);
}
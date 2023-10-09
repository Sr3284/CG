import sys
import numpy as np
import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GL import shaders

vao = None
vbo = None
shaderProgram = None

def readShaderFile(filename):
    with open('shader/' + filename, 'r') as myfile:
        return myfile.read()

def init():
    global shaderProgram
    global vao
    global vbo

    glClearColor(0, 0, 0, 0)

    vertex_code = readShaderFile('hello.vp')
    fragment_code = readShaderFile('hello.fp')

    # Initialize Pygame
    pygame.init()
    display = (800, 600)
    pygame.display.set_mode(display, DOUBLEBUF | OPENGL)
    pygame.display.set_caption("Pygame OpenGL")

    # compile shaders and program
    vertexShader = shaders.compileShader(vertex_code, GL_VERTEX_SHADER)
    fragmentShader = shaders.compileShader(fragment_code, GL_FRAGMENT_SHADER)
    shaderProgram = shaders.compileProgram(vertexShader, fragmentShader)

    # Create and bind the Vertex Array Object
    vao = glGenVertexArrays(1)
    glBindVertexArray(vao)

    # Create and bind the Vertex Buffer Object
    vertices = np.array([[0, 1, 0], [-1, -1, 0], [1, -1, 0]], dtype='f')
    vbo = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)

    glVertexAttribPointer(0, 3, GL_FLOAT, False, 0, None)  # first 0 is the location in shader
    glBindAttribLocation(shaderProgram, 0, 'vertexPosition')  # name of attribute in shader
    glEnableVertexAttribArray(0)  # 0=location do atributo

    # Note that this is allowed, the call to glVertexAttribPointer registered VBO
    # as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0)
    glBindVertexArray(0)

def display():
    global shaderProgram
    global vao

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    # load everything back
    glUseProgram(shaderProgram)
    glBindVertexArray(vao)

    # glDrawArrays( mode , first, count)
    glDrawArrays(GL_TRIANGLES, 0, 3)

    # clean things up
    glBindVertexArray(0)
    glUseProgram(0)

    pygame.display.flip()

def reshape(width, height):
    glViewport(0, 0, width, height)

if __name__ == '__main__':
    init()

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        display()


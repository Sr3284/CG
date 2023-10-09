import java.nio.FloatBuffer;

import javax.swing.JFrame;

import com.jogamp.common.nio.Buffers;
import com.jogamp.opengl.GL;
import com.jogamp.opengl.GL3;
import com.jogamp.opengl.GLAutoDrawable;
import com.jogamp.opengl.GLCapabilities;
import com.jogamp.opengl.GLEventListener;
import com.jogamp.opengl.GLException;
import com.jogamp.opengl.GLProfile;
import com.jogamp.opengl.awt.GLJPanel;
import com.jogamp.opengl.util.GLBuffers;
import com.jogamp.opengl.util.glsl.ShaderCode;
import com.jogamp.opengl.util.glsl.ShaderProgram;

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
public class Hello implements GLEventListener {

	private ShaderProgram shaderProgram;
	private ShaderCode vertexShader;
	private ShaderCode fragmentShader;

	private int[] vao = new int[1];
	private int[] vbo = new int[1];

	@Override
	public void init(GLAutoDrawable canvas) {
		GL3 gl = canvas.getGL().getGL3();
		gl.glClearColor(0f, 0f, 0f, 1f);
		// Create and compile our GLSL program from the shaders
		createShaders(gl);
		// Create and Bind the Vertex Array Object first, then bind and set vertex
		// buffer(s) and attribute pointer(s).
		gl.glGenVertexArrays(1, vao, 0);
		gl.glBindVertexArray(vao[0]);
		// Set up vertex data (and buffer(s)) and attribute pointers
		float vertexData[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
		FloatBuffer vertexDataBuffer = GLBuffers.newDirectFloatBuffer(vertexData);
		gl.glGenBuffers(1, vbo, 0);
		gl.glBindBuffer(GL.GL_ARRAY_BUFFER, vbo[0]);
		gl.glBufferData(GL.GL_ARRAY_BUFFER, Buffers.SIZEOF_FLOAT * vertexDataBuffer.limit(), vertexDataBuffer,
				GL.GL_STATIC_DRAW);
	}

	@Override
	public void display(GLAutoDrawable canvas) {
		System.out.println("Display: " + canvas);

		GL3 gl = canvas.getGL().getGL3();
		gl.glClear(GL.GL_DEPTH_BUFFER_BIT | GL.GL_COLOR_BUFFER_BIT);

		gl.glUseProgram(shaderProgram.program());
		// tem que ativar todos os atributos, inicialmente sao desabilitados por padrao
		gl.glEnableVertexAttribArray(0);
		gl.glBindBuffer(GL.GL_ARRAY_BUFFER, vbo[0]);
		// 0=location do atributo no shader
		// penultimo zero pode ser 3 * Float.BYTES
		gl.glVertexAttribPointer(0, 3, GL.GL_FLOAT, false, 0, 0);// first 0 is the location in shader
		// Draw our first triangle
		gl.glDrawArrays(GL.GL_TRIANGLES, 0, 3);// or drawElements

		gl.glDisableVertexAttribArray(0);

		gl.glDetachShader(shaderProgram.program(), vertexShader.id());
		gl.glDetachShader(shaderProgram.program(), fragmentShader.id());
		// when you have finished drawing everything that you want using the
		// shaders, call this to stop further shader interactions.
		gl.glUseProgram(0);
	}

	@Override
	public void reshape(GLAutoDrawable canvas, int x, int y, int width, int height) {
		System.out.println("Reshape: " + canvas);

		GL3 gl3 = canvas.getGL().getGL3();
		gl3.glViewport(x, y, width, height);
	}

	@Override
	public void dispose(GLAutoDrawable canvas) {
		System.out.println("Dispose: " + canvas);

		GL3 gl3 = canvas.getGL().getGL3();
		gl3.glDeleteBuffers(1, vbo, 0);
		gl3.glDeleteVertexArrays(1, vao, 0);

		gl3.glDeleteProgram(shaderProgram.id());
		gl3.glDeleteShader(vertexShader.id());
		gl3.glDeleteShader(fragmentShader.id());
	}

	private void createShaders(GL3 gl) {
		vertexShader = ShaderCode.create(gl, GL3.GL_VERTEX_SHADER, getClass(), "shader", null, "hello", false);
		fragmentShader = ShaderCode.create(gl, GL3.GL_FRAGMENT_SHADER, getClass(), "shader", null, "hello", false);

		if (!vertexShader.compile(gl, System.err))
			throw new GLException("Couldn't compile shader: " + vertexShader);
		if (!fragmentShader.compile(gl, System.err))
			throw new GLException("Couldn't compile shader: " + fragmentShader);

		shaderProgram = new ShaderProgram();
		shaderProgram.add(gl, vertexShader, System.err);
		shaderProgram.add(gl, fragmentShader, System.err);
		shaderProgram.link(gl, System.out);
	}

	public static void main(String[] args) {
		GLProfile profile = GLProfile.get(GLProfile.GL3);

		GLJPanel canvas = new GLJPanel(new GLCapabilities(profile));
		canvas.addGLEventListener(new Hello());

		JFrame app = new JFrame("Hello");
		app.setSize(480, 480);
		app.setLocationRelativeTo(null);
		app.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		app.getContentPane().add(canvas);
		app.setVisible(true);
	}
}

package com.apress.aviplayer;

import java.util.concurrent.atomic.AtomicBoolean;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Bundle;

/**
 * AVI player through OpenGL.
 * 
 * @author Onur Cinar
 */
public class OpenGLPlayerActivity extends AbstractPlayerActivity {
	/** Is playing. */
	private final AtomicBoolean isPlaying = new AtomicBoolean();

	/** Native renderer. */
	private long instance;
	
	/** GL surface view instance. */
	private GLSurfaceView glSurfaceView;
	
	/**
	 * On create.
	 * 
	 * @param savedInstanceState saved state.
	 */
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_gl_player);
        
        glSurfaceView = (GLSurfaceView) findViewById(R.id.gl_surface_view);
        
        // Set renderer
        glSurfaceView.setRenderer(renderer);
        
        // Render frame when requested
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }
    
    /**
     * On start.
     */
	protected void onStart() {
		super.onStart();
		
		// Initializes the native renderer
		instance = init(avi);
	}

	/**
     * On resume.
     */
	protected void onResume() {
		super.onResume();
		
		// GL surface view must be notified when activity is resumed
		glSurfaceView.onResume();
	}

	/**
	 * On pause.
	 */
	protected void onPause() {
		super.onPause();
		
		// GL surface view must be notified when activity is paused.
		glSurfaceView.onPause();
	}

	/**
	 * On stop.
	 */
	protected void onStop() {
		super.onStop();
		
		// Free the native renderer
		free(instance);
		instance = 0;
	}

	/**
	 * Request rendering based on the frame rate.
	 */
	private final Runnable player = new Runnable() {
		public void run() {
			// Calculate the delay using the frame rate
			long frameDelay = (long) (1000 / getFrameRate(avi));
			
			// Start rendering while playing
			while (isPlaying.get()) {
				// Request rendering
				glSurfaceView.requestRender();
				
				// Wait for the next frame
				try {
					Thread.sleep(frameDelay);
				} catch (InterruptedException e) {
					break;
				}
			}
		}
	};
	
	/**
	 * OpenGL renderer.
	 */
	private final Renderer renderer = new Renderer() {
		public void onDrawFrame(GL10 gl) {
			// Render the next frame
			if (!render(instance, avi))
			{
				isPlaying.set(false);
			}
		}

		public void onSurfaceChanged(GL10 gl, int width, int height) {
			
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			// Initialize the OpenGL surface
			initSurface(instance, avi);
			
			// Start playing since surface is ready
			isPlaying.set(true);

			// Start player
			new Thread(player).start();
		}
	};
	
	/**
	 * Initializes the native renderer.
	 * 
	 * @param avi file descriptor.
	 * @return native instance.
	 */
	private native static long init(long avi);
	
	/**
	 * Initializes the OpenGL surface.
	 * 
	 * @param instance native instance.
	 */
	private native static void initSurface(long instance, long avi);
	
	/**
	 * Renders the frame from given AVI file descriptor.
	 * 
	 * @param instance native instance.
	 * @param avi file descriptor.
	 * @return true if there are more frames, false otherwise.
	 */
	private native static boolean render(long instance, long avi);
	
	/**
	 * Free the native renderer.
	 * 
	 * @param instance native instance.
	 */
	private native static void free(long instance);
}

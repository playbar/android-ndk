package com.apress.wavplayer;

import java.io.File;
import java.io.IOException;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

/**
 * WAVE player main activity.
 * 
 * @author Onur Cinar
 */
public class MainActivity extends Activity implements OnClickListener {
	/** File name edit text. */
	private EditText fileNameEdit;

	/**
	 * On create.
	 * 
	 * @param savedInstanceState
	 *            saved state.
	 */
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		fileNameEdit = (EditText) findViewById(R.id.fileNameEdit);
		Button playButton = (Button) findViewById(R.id.playButton);
		playButton.setOnClickListener(this);
	}

	/**
	 * On click.
	 * 
	 * @param view
	 *            view instance.
	 */
	public void onClick(View view) {
		switch (view.getId()) {
		case R.id.playButton:
			onPlayButtonClick();
		}
	}

	/**
	 * On play button click.
	 */
	private void onPlayButtonClick() {
		// Under the external storage
		File file = new File(Environment.getExternalStorageDirectory(),
				fileNameEdit.getText().toString());

		// Start player
		PlayTask playTask = new PlayTask();
		playTask.execute(file.getAbsolutePath());
	}

	/**
	 * Play task.
	 */
	private class PlayTask extends AsyncTask<String, Void, Exception> {
		/**
		 * Background task.
		 * 
		 * @param file
		 *            WAVE file.
		 */
		protected Exception doInBackground(String... file) {
			Exception result = null;
			
			try {
				// Play the WAVE file
				play(file[0]);
			} catch (IOException ex) {
				result = ex;
			}
			
			return result;
		}

		/**
		 * Post execute.
		 * 
		 * @param ex
		 *            exception instance.
		 */
		protected void onPostExecute(Exception ex) {
			// Show error message if playing failed
			if (ex != null) {
				new AlertDialog.Builder(MainActivity.this)
						.setTitle(R.string.error_alert_title)
						.setMessage(ex.getMessage()).show();
			}
		}
	}

	/**
	 * Plays the given WAVE file using native sound API.
	 * 
	 * @param fileName
	 *            file name.
	 * @throws IOException
	 */
	private native void play(String fileName) throws IOException;
	
	static {
		System.loadLibrary("WAVPlayer");
	}
}

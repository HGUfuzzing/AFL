package edu.handong.service;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import com.google.auth.oauth2.GoogleCredentials;
import com.google.firebase.FirebaseApp;
import com.google.firebase.FirebaseOptions;

public class FirebaseInitialize {
	
	public void initialize() {

		try {
			FileInputStream serviceAccount = new FileInputStream("./serviceAccount.json");
			FirebaseOptions options;
			options = new FirebaseOptions.Builder()
				    .setCredentials(GoogleCredentials.getApplicationDefault())
				    .setDatabaseUrl("https://fuzz-web.firebaseio.com")
				    .build();
			
			FirebaseApp.initializeApp(options);
		} 
		catch (FileNotFoundException e1) {
			e1.printStackTrace();
		}
		catch (IOException e) {
			e.printStackTrace();
		}

	}
}





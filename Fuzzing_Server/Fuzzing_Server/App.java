package Fuzzing_Server.Fuzzing_Server;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

import edu.handong.service.*;

import org.json.*;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

/**
 * Hello world!
 *
 */
public class App {
	public static void main(String[] args) {
		
		//initialize firebase
		FirebaseInitialize firebase = new FirebaseInitialize();
		firebase.initialize();

		PrintWriter out = null;
		ServerSocket s_socket = null;

		Socket c_socket = null;
		String line;

		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		Date time = new Date();

		// JSON values to send
		String last_update = "";
		String clnt_address = "";
		int runtime = -1;
		int cycles_done = -1;
		int total_paths = -1;
		int unique_crashes = -1;
		int unique_hangs = -1;
		int pid = -1;
		
		//open server socket
		try {
			s_socket = new ServerSocket(10000);

		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
		} catch (IOException e1) {
			e1.printStackTrace();
		}

		
		try {
			// As an admin, the app has access to read and write all data, regardless of
			// Security Rules
			final FirebaseDatabase database = FirebaseDatabase.getInstance();
			DatabaseReference ref = database.getReference("");

			DatabaseReference usersRef = ref.child("fuzzing");

			while (true) {

				System.out.println("Waiting for client...");
				
				c_socket = s_socket.accept();
				clnt_address = c_socket.getInetAddress().toString();
				System.out.println("Client is connected! : " + clnt_address);
				
				//open json_recv file & ready for future input data from fuzz client
				out = new PrintWriter("./json_recv.txt");
				InputStream input_data = c_socket.getInputStream();
				InputStreamReader reader = new InputStreamReader(input_data, StandardCharsets.UTF_8);
				BufferedReader br = new BufferedReader(reader);
				
				
				
				//read input data line by line
				while ((line = br.readLine()) != null) {
					//write the line to "json_recv.txt"
					out.println(line);
					System.out.println(line);
					
					//save all input data
					JSONObject obj = new JSONObject(line);
					runtime = obj.getInt("runtime");
					cycles_done = obj.getInt("cycles done");
					total_paths = obj.getInt("total paths");
					unique_crashes = obj.getInt("uniq crashes");
					unique_hangs = obj.getInt("uniq hangs");
					pid = obj.getInt("pid");
					time = new Date();
					last_update = format.format(time);
				}
				
				
				
				/* Sending info to firebase */
				//key
				clnt_address = clnt_address.replace("/", "-");
				clnt_address = clnt_address.replace(".", " ");
				String fuzzerKey = Integer.toString(pid) + clnt_address;

				Map<String, Object> clients = new HashMap<String, Object>();
				clients.put(fuzzerKey, new FuzzingInfo(last_update, runtime, cycles_done, total_paths,
						unique_crashes, unique_hangs));
				usersRef.updateChildrenAsync(clients);

				
				out.close();
				c_socket.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			out.close();
			try {
				c_socket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}

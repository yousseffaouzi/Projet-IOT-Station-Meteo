package com.weather.stationtest;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;


import android.content.pm.PackageManager;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.text.SimpleDateFormat;
import java.util.Date;


public class MainActivity extends AppCompatActivity {

    public TextView temperature, humidity, pressure;
    private DatabaseReference ref;
    Button button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //check the connection to the Firebase database
        Toast.makeText(this, "Firebase connected successfully", Toast.LENGTH_LONG).show();
        //assign each variable declared to an actual textView
        temperature = (TextView) findViewById(R.id.tempLabel);
        humidity = (TextView) findViewById(R.id.humidLabel);
        pressure = (TextView) findViewById(R.id.pressureLabel);
        button = (Button) findViewById(R.id.button);
    }


    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        switch (requestCode){
            case 1:
                if(grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED)
                    requestLoc();
                return;
        }
    };

    private void requestLoc() {
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //create the connection to the firebase database
                ref = FirebaseDatabase.getInstance().getReference().child("FirebaseBranch");
                ref.addValueEventListener(new ValueEventListener() {
                    @Override
                    public void onDataChange(@NonNull DataSnapshot dataSnapshot) {


                        //get the date and hour
                        Date date = new Date(System.currentTimeMillis());
                        SimpleDateFormat dayFormatter= new SimpleDateFormat("dd_MM_yyyy");//use as dayFormatter.format(date)
                        SimpleDateFormat hourFormatter = new SimpleDateFormat("HH");//use as hourFormatter.format(date)

                        String tempS="", humidS="", pressS="";
                        Object temp, humid, press;
                        
                        if(hourFormatter.format(date).startsWith("0")) {

                            temp = dataSnapshot.child("date_"+dayFormatter.format(date)).child("hour_"+hourFormatter.format(date).charAt(1)).child("temperature").getValue();
                            humid = dataSnapshot.child("date_"+dayFormatter.format(date)).child("hour_"+hourFormatter.format(date).charAt(1)).child("humidity").getValue();
                            press = dataSnapshot.child("date_"+dayFormatter.format(date)).child("hour_"+hourFormatter.format(date).charAt(1)).child("pressure").getValue();

                        }
                        else{
                            temp = dataSnapshot.child("date_"+dayFormatter.format(date)).child("hour_"+hourFormatter.format(date)).child("temperature").getValue();
                            humid = dataSnapshot.child("date_"+dayFormatter.format(date)).child("hour_"+hourFormatter.format(date)).child("humidity").getValue();
                            press = dataSnapshot.child("date_"+dayFormatter.format(date)).child("hour_"+hourFormatter.format(date)).child("pressure").getValue();

                        }

                        if (temp != null) tempS = temp.toString(); else tempS = "0";
                        if(humid != null) humidS = humid.toString(); else humidS = "0";
                        if(press != null) pressS = press.toString(); else pressS = "0";


                        tempS += " °C";
                        humidS += " %";
                        pressS += " Pa";


                        //set text to the newly read data
                        temperature.setText(tempS);
                        humidity.setText(humidS);
                        pressure.setText(pressS);


                    }


                    @Override
                    public void onCancelled(@NonNull DatabaseError databaseError) {
                      
                    }
                });
            }
        });

    }
}
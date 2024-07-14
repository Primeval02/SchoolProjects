package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private float y1,y2;
    static final int MIN_DISTANCE = 720;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    //BUTTON EVENTS
    public void onClick(View view) {
        TextView text = (TextView) findViewById(R.id.textView);
        text.setText("Bradley Tate's app");
        Toast.makeText (MainActivity.this, "by Bradley Tate", Toast.LENGTH_LONG).show();
    }

    //SWIPE TO QUIT W/O LIFT FINGER
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch(event.getAction())
        {
            case MotionEvent.ACTION_DOWN:
                y1 = event.getY();
                break;
            case MotionEvent.ACTION_MOVE:
                y2 = event.getY();
                float deltaY = y2 - y1;
                if (deltaY > MIN_DISTANCE)
                {
                    System.exit(0);
                }
                break;
        }
        return super.onTouchEvent(event);
    }
}
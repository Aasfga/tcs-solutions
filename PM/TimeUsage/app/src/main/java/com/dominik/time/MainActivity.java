package com.dominik.time;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity
{
    private static final String FRONT_TIME = "FRONT_TIME";
    private static final String TOTAL_TIME = "TOTAL_TIME";
    private static final String RUN_COUNT = "RUN_COUNT";
    private static final String TICK = "TICK";
    private boolean isVisible = false;
    private int runCount;
    private int totalTime;
    private int frontTime;
    BroadcastReceiver receiver;

    private void showData()
    {
        ((TextView) findViewById(R.id.totalTime)).setText(String.valueOf(totalTime));
        ((TextView) findViewById(R.id.frontTime)).setText(String.valueOf(frontTime));
        ((TextView) findViewById(R.id.runCount)).setText(String.valueOf(runCount));
    }

    private void retrieveNumbers(Bundle savedInstanceState)
    {
        runCount = savedInstanceState.getInt(RUN_COUNT);
        totalTime = savedInstanceState.getInt(TOTAL_TIME);
        frontTime = savedInstanceState.getInt(FRONT_TIME);
    }

    private void retrieveNumbers(SharedPreferences preferences)
    {
        runCount = preferences.getInt(RUN_COUNT, 0) + 1;
        totalTime = preferences.getInt(TOTAL_TIME, 0);
        frontTime = preferences.getInt(FRONT_TIME, 0);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        if(receiver == null)
        {
            receiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    frontTime += (isVisible ? 1 : 0);
                    totalTime++;
                    showData();
                }
            };
            LocalBroadcastManager.getInstance(this).registerReceiver(receiver, new IntentFilter(TICK));
            SleepService.startTicking(this);
        }

        if(savedInstanceState != null)
            retrieveNumbers(savedInstanceState);
        else
            retrieveNumbers(getPreferences(MODE_PRIVATE));
        setContentView(R.layout.activity_main);
        showData();
    }

    @Override
    public void onRestoreInstanceState(Bundle savedInstanceState)
    {
        super.onRestoreInstanceState(savedInstanceState);
    }

    @Override
    public void onSaveInstanceState(Bundle outState)
    {
        super.onSaveInstanceState(outState);
        outState.putInt(FRONT_TIME, frontTime);
        outState.putInt(TOTAL_TIME, totalTime);
        outState.putInt(RUN_COUNT, runCount);
    }

    @Override
    protected void onStart()
    {
        super.onStart();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        isVisible = true;
    }

    @Override
    protected void onPause()
    {
        super.onPause();
    }

    @Override
    protected void onStop()
    {
        super.onStop();
        isVisible = false;
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        SharedPreferences.Editor editor = getPreferences(MODE_PRIVATE).edit();
        editor.putInt(RUN_COUNT, runCount);
        editor.putInt(TOTAL_TIME, totalTime);
        editor.putInt(FRONT_TIME, frontTime);
        editor.apply();
        LocalBroadcastManager.getInstance(this).unregisterReceiver(receiver);
        SleepService.stopTicking(this);
    }
}

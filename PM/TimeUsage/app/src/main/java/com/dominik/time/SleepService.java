package com.dominik.time;

import android.app.IntentService;
import android.content.Intent;
import android.content.Context;
import android.support.v4.content.LocalBroadcastManager;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class SleepService extends IntentService
{
    private static final String TICK = "TICK";
    CountDownLatch latch;
    private LocalBroadcastManager broadcast = LocalBroadcastManager.getInstance(this);

    private void sendTick()
    {
        Intent intent = new Intent(TICK);
        broadcast.sendBroadcast(intent);
    }

    public SleepService() {
        super("SleepService");
    }

    public static void startTicking(Context context)
    {
        Intent intent = new Intent(context, SleepService.class);
        intent.setAction(TICK);
        context.startService(intent);
    }

    public static void stopTicking(Context context)
    {
        Intent intent = new Intent(context, SleepService.class);
        context.stopService(intent);
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null)
        {
            final String action = intent.getAction();
            if(TICK.equals(action))
                handleTick();
        }
    }

    private void handleTick()
    {
        latch = new CountDownLatch(1);
        try
        {
            while(latch.getCount() != 0)
            {
                latch.await(1, TimeUnit.SECONDS);
                sendTick();
            }
        }
        catch (InterruptedException ignored)
        {}
    }

    @Override
    public void onDestroy()
    {
        latch.countDown();
    }
}


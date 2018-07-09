package com.dominik.checker

import android.app.*
import android.app.AlarmManager.ELAPSED_REALTIME_WAKEUP
import android.app.PendingIntent.FLAG_CANCEL_CURRENT
import android.content.Intent
import android.content.Context
import android.os.SystemClock
import android.support.v4.app.NotificationCompat.PRIORITY_DEFAULT
import android.support.v4.app.NotificationManagerCompat
import android.support.v4.content.LocalBroadcastManager
import android.util.Log
import com.android.volley.Request.Method.GET
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import com.dominik.checker.database.AppDatabase
import com.dominik.checker.database.Website

class CheckerService : IntentService("CheckerService") {

    private val websitesDb by lazy { AppDatabase.getDatabase(applicationContext).websites() }

    private val broadcast by lazy { LocalBroadcastManager.getInstance(this) }

    private val queue by lazy { Volley.newRequestQueue(this) }

    private val notificationManager by lazy { NotificationManagerCompat.from(this) }

    private fun setAlarm() {
        Log.d(TAG, "Setting alarm")
        val manager = applicationContext.getSystemService(ALARM_SERVICE) as AlarmManager
        val intent = Intent(CHECK, null, applicationContext, CheckerService::class.java)
        val alarmIntent = PendingIntent.getService(applicationContext, 0, intent, FLAG_CANCEL_CURRENT)
        manager.set(ELAPSED_REALTIME_WAKEUP, SystemClock.elapsedRealtime() + 10 * 1000, alarmIntent)
    }

    override fun onCreate() {
        super.onCreate()
        setAlarm()
    }

    override fun onHandleIntent(intent: Intent?) {
        if (intent == null)
            return

        when (intent.action) {
            ADD_ITEM -> handleAddItem(intent.getStringExtra(ITEM))
            DELETE_ITEM -> handleDeleteItem(intent.getStringExtra(ITEM))
            BROADCAST -> broadcastList()
            CHECK -> handleCheck()
        }
    }

    private fun handleAddItem(url: String) {
        Log.d(TAG, "Adding url: $url")
        websitesDb.insert(Website(url, ""))
        val request = StringRequest(GET, url,
                {
                    websitesDb.insert(Website(url, it))
                    broadcastList()
                },
                {
                    Log.w(TAG, it.cause)
                })
        queue.add(request)
    }

    private fun handleDeleteItem(item: String) {
        Log.d(TAG, "Deleting item: $item")
        websitesDb.delete(item)
        broadcastList()
    }

    private fun broadcastList() {
        Log.d(TAG, "Broadcasting list")
        val array = ArrayList<String>(websitesDb.getAddresses())
        val intent = Intent(BROADCAST)
        intent.putExtra(LIST, array)
        broadcast.sendBroadcast(intent)
    }

    private fun showNotification(url: String) {
        val builder = Notification.Builder(this)
                .setSmallIcon(R.drawable.notification_icon_background)
                .setContentTitle("Checker")
                .setContentText(url)
                .setPriority(PRIORITY_DEFAULT)
        notificationManager.notify(url.hashCode(), builder.build())
    }


    private fun handleCheck() {
        Log.d(TAG, "Handle check")
        val websites = websitesDb.getAll()
        for( site in websites) {
            val request = StringRequest(GET, site.address,
                    {
                        if(it != site.body)
                        {
                            websitesDb.insert(Website(site.address, it))
                            showNotification(site.address)
                            Log.d(TAG, "Website ${site.address} changed")
                        }
                    },
                    {
                        Log.w(TAG, it.cause)
                    })
            queue.add(request)
        }
        setAlarm()
    }

    companion object {
        const val DELETE_ITEM = "com.dominik.action.DELETE_ITEM"
        const val ADD_ITEM = "com.dominik.action.ADD_ITEM"
        const val BROADCAST = "com.dominik.action.BROADCAST"
        const val CHECK = "com.dominik.action.CHECK"

        const val ITEM = "com.dominik.extra.ITEM"
        const val LIST = "com.dominik.extra.LIST"

        private const val TAG = "SiteCheckerService"
        private const val DATA_NAME = "checkerData"

        @Volatile private var counter = 0




        fun startActionAdd(context: Context, item: String) {
            if (item == "")
                return
            val intent = Intent(context, CheckerService::class.java)
            intent.action = ADD_ITEM
            intent.putExtra(ITEM, item)
            context.startService(intent)
        }

        fun startActionDelete(context: Context, item: String) {
            val intent = Intent(context, CheckerService::class.java)
            intent.action = DELETE_ITEM
            intent.putExtra(ITEM, item)
            context.startService(intent)
        }

        fun startActionBroadcast(context: Context) {
            val intent = Intent(context, CheckerService::class.java)
            intent.action = BROADCAST
            context.startService(intent)
        }
    }
}

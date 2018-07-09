package com.dominik.checker

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.graphics.Color
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.support.v4.content.LocalBroadcastManager
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import com.dominik.checker.CheckerService.Companion.BROADCAST
import com.dominik.checker.CheckerService.Companion.LIST


const val TAG = "MainActivity"


class MainActivity : AppCompatActivity() {

    private val array = ArrayList<String>()
    private lateinit var adapter: UrlAdapter
    lateinit var list: ListView
    lateinit var editText: EditText
    lateinit var addButton: Button
    private val receiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent?) {
            if (intent == null)
                return
            when (intent.action) {
                BROADCAST -> handleBroadcast(intent)
                else -> Log.w(TAG, "Strange intent with action: '${intent.action}' was received")
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        list = findViewById(R.id.list)
        editText = findViewById(R.id.edit_text)
        addButton = findViewById(R.id.add_button)

        adapter = UrlAdapter(this, R.id.list_item, array)
        list.adapter = adapter
        LocalBroadcastManager.getInstance(this).registerReceiver(receiver, IntentFilter(BROADCAST))

        CheckerService.startActionBroadcast(this)
    }

    fun handleBroadcast(intent: Intent) {
        val arrayList = intent.getSerializableExtra(LIST) as ArrayList<*>
        array.clear();
        arrayList.forEach { array.add(it as String) }
        adapter.notifyDataSetChanged()
    }

    fun addButtonClick(view: View?) {
        val url = editText.text.toString()
        editText.text.clear()
        CheckerService.startActionAdd(this, url)
        if(!array.contains(url))
        {
            array.add(url)
            adapter.notifyDataSetChanged()
        }
    }

    companion object {
        class UrlAdapter(context: Context?, resource: Int, list: ArrayList<String>?) : ArrayAdapter<String>(context, resource, list) {
            override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
                val item = getItem(position)
                val view = convertView
                        ?: LayoutInflater.from(context).inflate(R.layout.list_item, null)

                val url = view.findViewById<TextView>(R.id.url_name)
                val button = view.findViewById<ImageButton>(R.id.remove_button)
                url.text = item
                button.setOnClickListener {
                    CheckerService.startActionDelete(context, item)
                }
                view.setBackgroundColor(Color.parseColor(if (position % 2 == 0) "#FFFFFF" else "#DCE2DF"))
                return view
            }
        }

    }
}

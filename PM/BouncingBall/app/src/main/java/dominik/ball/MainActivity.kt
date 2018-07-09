package dominik.ball

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.support.v7.app.AppCompatActivity
import android.os.Bundle

class MainActivity : AppCompatActivity() {

    val ballView: BallView by lazy { findViewById<BallView>(R.id.ballView) }
    private lateinit var manager: SensorManager
    private val listener = object : SensorEventListener {
        override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {
        }

        override fun onSensorChanged(event: SensorEvent) {
            val x = event.values?.get(0) ?: 0f
            val y = event.values?.get(1) ?: 0f
            ballView.setAcceleration(x, y)
        }

    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    override fun onStart() {
        super.onStart()
        manager = getSystemService(Context.SENSOR_SERVICE) as SensorManager
        manager.registerListener(listener, manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_GAME)
    }

    override fun onStop() {
        super.onStop()
        manager.unregisterListener(listener)
    }
}

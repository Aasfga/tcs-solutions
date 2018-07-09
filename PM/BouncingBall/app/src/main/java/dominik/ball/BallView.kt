package dominik.ball

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.os.Handler
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.MotionEvent.*
import android.view.VelocityTracker
import android.widget.ImageView

const val frameRate = 120
const val time = 1f / frameRate
const val kowalski = 600
const val radius = 150f
const val AXIS_X = 0
const val AXIS_Y = 1



class BallView(context: Context, attributeSet: AttributeSet) : ImageView(context, attributeSet) {
    private var pos = floatArrayOf(-1f, -1f)
    private var vel = floatArrayOf(0f, 0f)
    private var acc = floatArrayOf(0f, 0f)
    private var borders = floatArrayOf(0f, 0f)
    private val drawHandler = Handler()
    private val fillPaint = Paint()
    private val borderPaint = Paint()
    private val tracker = VelocityTracker.obtain()



    init {
        fillPaint.color = Color.parseColor("#31A2AC")
        fillPaint.style = Paint.Style.FILL

        borderPaint.color = Color.parseColor("#AF1C1C")
        borderPaint.style = Paint.Style.STROKE
        borderPaint.strokeWidth = 10f
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {

        val x = event.x
        val y = event.y

        when(event.action) {
            ACTION_DOWN, ACTION_MOVE -> {
                tracker.addMovement(event)
                setPosition(x, y)
            }
            ACTION_UP -> {
                tracker.computeCurrentVelocity(1000)
                setVelocity(tracker.xVelocity, tracker.yVelocity)
                setPosition(x, y)
                tracker.clear()
            }
        }

        return true
    }

    public fun setPosition(x: Float, y: Float) {
        pos[AXIS_X] = x
        pos[AXIS_Y] = y
    }

    public fun setVelocity(x: Float, y: Float) {
        vel[AXIS_X] = x
        vel[AXIS_Y] = y
    }

    public fun setAcceleration(x: Float, y: Float) {
        acc[AXIS_X] = x
        acc[AXIS_Y] = y
    }

    private fun makeMoveIn(d: Int) {
        val delta = vel[d] * time
        when {
            pos[d] - radius + delta < 0 -> {
                pos[d] = radius
                vel[d] *= -0.9f
            }
            pos[d] + radius + delta > borders[d] -> {
                pos[d] = borders[d] - radius
                vel[d] *= -0.9f
            }
            else -> pos[d] += vel[d] * time
        }
    }

    override fun onDraw(canvas: Canvas) {

        if (pos[0] == -1f) {
            pos[0] = width / 2f; borders[0] = width.toFloat()
            pos[1] = height / 2f; borders[1] = height.toFloat()
        }

        vel[0] += -time * acc[0] * kowalski
        vel[1] += time * acc[1] * kowalski


        makeMoveIn(AXIS_X)
        makeMoveIn(AXIS_Y)
        canvas.drawCircle(pos[0], pos[1], radius, fillPaint)
        canvas.drawCircle(pos[0], pos[1], radius, borderPaint)
        drawHandler.postDelayed({ invalidate() }, 1000L / frameRate)
    }


}
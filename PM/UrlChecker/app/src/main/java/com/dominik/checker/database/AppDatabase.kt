package com.dominik.checker.database

import android.arch.persistence.room.Database
import android.arch.persistence.room.Room
import android.arch.persistence.room.RoomDatabase
import android.content.Context

@Database(entities = [(Website::class)], version = 1)
abstract class AppDatabase : RoomDatabase() {
    abstract fun websites(): WebsiteDao

    companion object {
        private var appDatabase: AppDatabase? = null

        fun getDatabase(context: Context): AppDatabase {
            if (appDatabase == null)
                appDatabase = Room.databaseBuilder(context.applicationContext, AppDatabase::class.java, "appDatabase").allowMainThreadQueries().build()
            return appDatabase as AppDatabase
        }

    }
}
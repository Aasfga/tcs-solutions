package com.dominik.checker.database

import android.arch.persistence.room.Dao
import android.arch.persistence.room.Delete
import android.arch.persistence.room.Insert
import android.arch.persistence.room.OnConflictStrategy.REPLACE
import android.arch.persistence.room.Query

@Dao
interface WebsiteDao {

    @Query("SELECT address FROM website")
    fun getAddresses() : List<String>

    @Query("SELECT body FROM website WHERE address LIKE :address")
    fun getBody(address: String) : String

    @Query("SELECT * FROM website")
    fun getAll() : List<Website>

    @Insert(onConflict = REPLACE)
    fun insert(website: Website)

    @Query("DELETE FROM website WHERE address LIKE :address")
    fun delete(address: String)
}
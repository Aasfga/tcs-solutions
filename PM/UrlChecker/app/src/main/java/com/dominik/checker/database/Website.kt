package com.dominik.checker.database

import android.arch.persistence.room.ColumnInfo
import android.arch.persistence.room.Entity
import android.arch.persistence.room.PrimaryKey

@Entity
class Website(@PrimaryKey var address: String,
              @ColumnInfo var body: String)
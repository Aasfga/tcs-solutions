CREATE TABLE tab(
	a int, 
	b char(3) NOT NULL CHECK (b IN ('tak', 'nie'))
);
INSERT INTO tab VALUES (1,'tak'), (2,'nie'), (3,'nie'), (4,'tak'), (5,'tak');
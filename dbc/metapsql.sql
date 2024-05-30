SELECT numeric_precision  
FROM information_schema.columns 
WHERE table_name = 'oscar' and column_name = 'name';
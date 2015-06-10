CREATE OR REPLACE FUNCTION map_add(v_name varchar,
				   v_minx numeric,
				   v_miny numeric,
				   v_maxx numeric,
				   v_maxy numeric,
				   v_srid integer)
RETURNS integer AS $$
<< outerblock >>
DECLARE
	map_id integer := -1;
BEGIN

    insert into g_map
					(m_name, minx, miny, maxx, maxy,srid)
			values
					(v_name, v_minx, v_miny, v_maxx, v_maxy, v_srid)
			returning gid into map_id;
    RETURN map_id;
END;
$$ LANGUAGE plpgsql;
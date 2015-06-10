CREATE OR REPLACE FUNCTION map_update(
	v_gid integer,
	v_name varchar,
	v_minx numeric,
	v_miny numeric,
	v_maxx numeric,
	v_maxy numeric,
	v_srid integer)
RETURNS integer AS $$
<< outerblock >>
DECLARE
	DECLARE v_version integer := 1;
BEGIN

    select version into v_version from g_map where gid=v_gid;
    update g_map set m_name = v_name,
			   minx = v_minx,
			   miny = v_miny,
			   maxx = v_maxx,
			   maxy = v_maxy,
			   srid = v_srid,
			   version=v_version+1
    where
		gid = v_gid;

    RETURN 0;
END;
$$ LANGUAGE plpgsql;
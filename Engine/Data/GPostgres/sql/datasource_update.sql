CREATE OR REPLACE FUNCTION gf_datasource_update(
	v_gid integer,
	v_name varchar,
	v_engine varchar,
	v_uri varchar)
RETURNS boolean AS $$
<< outerblock >>
DECLARE
	DECLARE v_version integer := 1;
	DECLARE v_ret boolean := TRUE;
BEGIN

    select version into v_version from g_data_source where gid=v_gid;
    update g_data_source set
					name = v_name,
					engine = v_engine,
					uri = v_uri,
					version=v_version+1
    where
		gid = v_gid;

    RETURN v_ret;
END;
$$ LANGUAGE plpgsql;
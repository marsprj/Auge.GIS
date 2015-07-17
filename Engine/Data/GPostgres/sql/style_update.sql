CREATE OR REPLACE FUNCTION gf_style_update(
	v_gid integer,
	v_name varchar,
	v_type varchar,
	v_text varchar,
	v_user integer)
RETURNS boolean AS $$
<< outerblock >>
DECLARE
	DECLARE v_version integer := 1;
	DECLARE v_ret boolean := TRUE;
BEGIN

    select version into v_version from g_style where gid=v_gid;
    update g_style set
					s_text = v_text,
					s_type = v_type,
					version=v_version+1
    where
		name = v_name and user_id = v_user;

    RETURN v_ret;
END;
$$ LANGUAGE plpgsql;




CREATE OR REPLACE FUNCTION gf_style_update(
	v_gid integer,
	v_name varchar,
	v_text varchar,
	v_user integer)
RETURNS boolean AS $$
<< outerblock >>
DECLARE
	DECLARE v_version integer := 1;
	DECLARE v_ret boolean := TRUE;
BEGIN

    select version into v_version from g_style where s_name=v_name;
    update g_style set
					s_text = v_text,
					version=v_version+1
    where
		s_name = v_name and user_id = v_user;

    RETURN v_ret;
END;
$$ LANGUAGE plpgsql;


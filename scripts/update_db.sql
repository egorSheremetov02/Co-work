DROP TABLE projects CASCADE;
DROP TABLE tasks CASCADE;
DROP TABLE users CASCADE;
DROP TABLE actions CASCADE;
DROP TABLE attached_files CASCADE;
DROP TABLE dependence_project_user CASCADE;
DROP TABLE dependence_task_user CASCADE;


CREATE SEQUENCE "public".projects_id_seq START WITH 1;

CREATE SEQUENCE "public".tasks_id_seq START WITH 1;

CREATE SEQUENCE "public".users_id_seq START WITH 1;

CREATE  TABLE "public".projects (
	id                   integer DEFAULT nextval('projects_id_seq'::regclass) NOT NULL ,
	name                 varchar(100)   ,
	due_date             date   ,
	start_date           date DEFAULT current_date  ,
	CONSTRAINT pk_projects_id PRIMARY KEY ( id )
 );

CREATE  TABLE "public".tasks (
	id                   integer DEFAULT nextval('tasks_id_seq'::regclass) NOT NULL ,
	name                 varchar(100)   ,
	description          varchar   ,
	due_date             date   ,
	project_id           integer   ,
	urgency              integer   ,
	status               varchar(15)   ,
	start_date           date DEFAULT current_date  ,
	CONSTRAINT pk_tasks_id PRIMARY KEY ( id ),
	CONSTRAINT fk_tasks_projects FOREIGN KEY ( project_id ) REFERENCES "public".projects( id )
 );

CREATE  TABLE "public".users (
	id                   integer DEFAULT nextval('users_id_seq'::regclass) NOT NULL ,
	account_name         varchar(100)   ,
	full_name            varchar(100)   ,
	"password"           varchar   ,
	role_in_system       varchar   ,
	email                varchar   ,
	CONSTRAINT pk_users_id PRIMARY KEY ( id )
 );

CREATE  TABLE "public".actions (
	task_id              integer   ,
	type_of_action       varchar   ,
	object_of_action     json   ,
	date_of_action       date DEFAULT current_date  ,
	user_id              integer   ,
	CONSTRAINT fk_actions_tasks FOREIGN KEY ( task_id ) REFERENCES "public".tasks( id )
 );

CREATE  TABLE "public".attached_files (
	task_id              integer   ,
	path_to_file         varchar   ,
	filename             varchar(100)   ,
	CONSTRAINT fk_attached_files_tasks FOREIGN KEY ( task_id ) REFERENCES "public".tasks( id )
 );

CREATE  TABLE "public".dependence_project_user (
	project_id           integer   ,
	user_id              integer   ,
	CONSTRAINT fk_dependence_project FOREIGN KEY ( project_id ) REFERENCES "public".projects( id )   ,
	CONSTRAINT fk_dependence_user FOREIGN KEY ( user_id ) REFERENCES "public".users( id )
 );

CREATE  TABLE "public".dependence_task_user (
	task_id              integer   ,
	user_id              integer   ,
	role_of_user         varchar   ,
	CONSTRAINT fk_dependence_task FOREIGN KEY ( task_id ) REFERENCES "public".tasks( id )   ,
	CONSTRAINT fk_dependence_user FOREIGN KEY ( user_id ) REFERENCES "public".users( id )
 );

INSERT INTO "public".users( id, account_name, full_name, password, role_in_system, email ) VALUES ( 1, 'admin', 'Egor Suvorov', '65e84be33532fb784c48129675f9eff3a682b27168c0ea744b2cf58ee02337c5', 'admin', 'yeputons@hse.ru');

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
	start_date           date DEFAULT CURRENT_DATE  ,
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
	start_date           date DEFAULT CURRENT_DATE  ,
	CONSTRAINT pk_tasks_id PRIMARY KEY ( id )
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
	object_of_action     varchar   ,
	date_of_action       date DEFAULT CURRENT_DATE  ,
	user_id              integer
 );

CREATE  TABLE "public".attached_files (
	task_id              integer   ,
	path_to_file         varchar   ,
	filename             varchar(100)
 );

CREATE  TABLE "public".dependence_project_user (
	project_id           integer   ,
	user_id              integer
 );

CREATE  TABLE "public".dependence_task_user (
	task_id              integer   ,
	user_id              integer   ,
	role_of_user         varchar
 );

ALTER TABLE "public".actions ADD CONSTRAINT fk_actions_tasks FOREIGN KEY ( task_id ) REFERENCES "public".tasks( id );

ALTER TABLE "public".attached_files ADD CONSTRAINT fk_attached_files_tasks FOREIGN KEY ( task_id ) REFERENCES "public".tasks( id );

ALTER TABLE "public".dependence_project_user ADD CONSTRAINT fk_dependence_project FOREIGN KEY ( project_id ) REFERENCES "public".projects( id );

ALTER TABLE "public".dependence_project_user ADD CONSTRAINT fk_dependence_user FOREIGN KEY ( user_id ) REFERENCES "public".users( id );

ALTER TABLE "public".dependence_task_user ADD CONSTRAINT fk_dependence_task FOREIGN KEY ( task_id ) REFERENCES "public".tasks( id );

ALTER TABLE "public".dependence_task_user ADD CONSTRAINT fk_dependence_user FOREIGN KEY ( user_id ) REFERENCES "public".users( id );

ALTER TABLE "public".tasks ADD CONSTRAINT fk_tasks_projects FOREIGN KEY ( project_id ) REFERENCES "public".projects( id );

INSERT INTO "public".projects( id, name, due_date, start_date ) VALUES ( 1, 'Name', '2021-04-17', '2021-05-09');
INSERT INTO "public".tasks( id, name, description, due_date, project_id, urgency, status, start_date ) VALUES ( 1, 'Name', 'Description', '2021-04-17', 1, 10, 'TODO', '2021-05-09');
INSERT INTO "public".tasks( id, name, description, due_date, project_id, urgency, status, start_date ) VALUES ( 2, 'Task_task', 'Description', '2021-04-17', 1, 7, 'status', '2021-05-10');
INSERT INTO "public".users( id, account_name, full_name, password, role_in_system, email ) VALUES ( 1, 'admin', 'Egor Suvorov', '65e84be33532fb784c48129675f9eff3a682b27168c0ea744b2cf58ee02337c5', 'admin', 'yeputons@hse.ru');
INSERT INTO "public".users( id, account_name, full_name, password, role_in_system, email ) VALUES ( 2, 'irzuk', 'irzuk', null, null, 'irina@hse.ru');
INSERT INTO "public".actions( task_id, type_of_action, object_of_action, date_of_action, user_id ) VALUES ( 2, 'create', null, '2021-05-10', 1);

CREATE  TABLE "public".projects (
	id                   serial  NOT NULL ,
	name                 varchar(100)   ,
	due_date             date   ,
	CONSTRAINT pk_projects_id PRIMARY KEY ( id )
 );

CREATE  TABLE "public".tasks (
	id                   serial  NOT NULL ,
	name                 varchar(100)   ,
	description          varchar   ,
	due_date             date   ,
	project_id           integer   ,
	urgency              integer   ,
	status               varchar(15)   ,
	CONSTRAINT pk_tasks_id PRIMARY KEY ( id )
 );

CREATE  TABLE "public".users (
	id                   serial  NOT NULL ,
	account_name         varchar(100)   ,
	full_name            varchar(100)   ,
	"password"           varchar   ,
	role_in_system       varchar   ,
	CONSTRAINT pk_users_id PRIMARY KEY ( id )
 );

CREATE  TABLE "public".actions (
	task_id              integer   ,
	type_of_action       varchar   ,
	object_of_action     json   ,
	date_of_action       date   ,
	user_id              integer
 );

CREATE  TABLE "public".attached_files (
	task_id              integer   ,
	path_to_file         varchar
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

ALTER TABLE "public".dependence_task_user ADD CONSTRAINT fk_dependence_user_0 FOREIGN KEY ( user_id ) REFERENCES "public".users( id );

ALTER TABLE "public".tasks ADD CONSTRAINT fk_tasks_projects FOREIGN KEY ( project_id ) REFERENCES "public".projects( id );

INSERT INTO "public".users( id, account_name, full_name, password, role_in_system ) VALUES ( 1, 'admin', 'Egor Suvorov', 'qwerty', 'admin');

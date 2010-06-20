drop table if exists thing;
create table thing (
       id integer primary key,
       name varchar(255)
);
insert into thing (id, name) values (1, 'a');
insert into thing (id, name) values (2, 'b');
insert into thing (id, name) values (3, 'c');

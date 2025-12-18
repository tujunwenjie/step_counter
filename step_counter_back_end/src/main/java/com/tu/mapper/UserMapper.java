package com.tu.mapper;

import com.tu.pojo.entity.User;
import org.apache.ibatis.annotations.*;

import java.util.List;

@Mapper
public interface UserMapper {
    
    @Insert("INSERT INTO user(name, password, create_time) VALUES(#{name}, #{password}, NOW())")
    @Options(useGeneratedKeys = true, keyProperty = "id")
    int insert(User user);
    
    @Select("SELECT * FROM user WHERE name = #{name}")
    User selectByName(String name);
    
    @Select("SELECT * FROM user WHERE id = #{id}")
    User selectById(Integer id);
    
    @Select("SELECT * FROM user")
    List<User> selectAll();

    @Select("select * from user u order by is_online desc limit #{num}")
    List<User> selectWithLimit(int num);

    @Update("UPDATE user SET is_online = #{isOnline}, last_activity = NOW() WHERE id = #{id}")
    int updateOnlineStatus(@Param("id") Integer id, @Param("isOnline") Boolean isOnline);
    
    @Update("UPDATE user SET last_activity = NOW() WHERE id = #{id}")
    int updateLastActivity(Integer id);
    
    @Update("UPDATE user SET is_admin = #{isAdmin} WHERE id = #{id}")
    int updateAdminStatus(@Param("id") Integer id, @Param("isAdmin") Boolean isAdmin);

    @Update("update user u set u.name = #{name},u.password=#{password} where id = #{userId}")
    int updateUser(@Param("userId") Integer userId, @Param("name") String name, @Param("password") String password);
    
    @Select("SELECT COUNT(*) FROM user")
    int countUsers();
}
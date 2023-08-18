# redis++库的使用

## 一、hash

1. #### 设置

   ```c++
   m_redis->hset(key, account, chatinfo);
   ```

   

2. #### 删除

   ```c++
   m_redis->hdel(key2, m_account);
   ```

   

3. #### 取值

   ```c++
   string groupname =  m_redis->hget("Group_" + m_groupid, "groupname").value();
   ```

   

4. #### 得到哈希全部内容

   ```c++
    m_redis->hgetall(key, std::inserter(m_userGroups,m_userGroups.begin())); 
   ```

   

## 二、set

1. #### 添加

   ```c++
   m_redis->sadd(m_groupid + "_Administrator", account);m_redis->hset(key, account, chatinfo);
   ```

2. #### 删除

   ```c++
   m_redis->srem(m_groupid + "_Member", account);
   ```

3. #### 判断元素是否在集合中

   ```c++
   bool exists2 = m_redis->sismember(m_groupid + "_Member", account);
   ```

4. #### 取交集

   ```c++
   string set1 = m_groupid + "_Member";
   string online_users = "ONLINE_USERS";
   m_redis->sinter( {online_users, set1}, std::inserter(groupMemberOnline, groupMemberOnline.begin()));
   ```

5. #### 获取集合中的所有成员

   ```c++
   std::unordered_set<std::string> set_members;
   redis_client.smembers("myset", std::inserter(set_members, set_members.begin()));
   ```

6. #### 删除集合

   ```c++
   m_redis->del(m_groupid + m_groupid + "_Administrator");
   ```

   

## 三、list

1. #### 添加

   - 添加到队尾

   ```c++
   m_redis->rpush(key, chatmsg);
   ```

   - 添加到队首

   ```c++
   m_redis->lpush(key, chatmsg);
   ```

2. #### 取出队列所有元素

   ```c++
   std::vector<std::string> msg;
   m_redis->lrange(key, 0, -1, std::back_inserter(msg));
   ```

3. #### 得到特定下标的元素

   ```c++
   string msg = m_redis->lindex(key, number).value();
   ```

4. #### 修改特定下标的元素值

   ```c++
   m_redis->lset(key, number, infostr);
   ```

5. #### 得到队列长度/元素个数

   ```c++
   readmsgcnt = m_redis->llen(m_groupid + "_Chat");
   ```

   


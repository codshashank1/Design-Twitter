class Tweet{
public:
    int id, time;
    // link to next tweet object.
    Tweet* next;

    Tweet(int i, int t) : id(i), time(t), next(NULL) {}
};

class User{
        
public:

    int id;
    Tweet *tweet_head;
    unordered_set<int> follows; 

    User() {}

    User(int i) : id(i), tweet_head(NULL) {
        follow(i);
    }

    void follow(int i) {
        follows.insert(i);          
    }

    void unfollow(int i) {
        follows.erase(i);
    }
    void post(int tweetId, int time) {
        Tweet* obj = new Tweet(tweetId, time);
        obj -> next = tweet_head;
        tweet_head = obj;
    }
};

class Twitter {
private:
    int timestamp = 0;   
    unordered_map<int, User> UserMap;

    
public:
    Twitter() {}

    void postTweet(int userId, int tweetId) {
        if(UserMap.find(userId) == UserMap.end()) {
            
            UserMap[userId] = User(userId);;
        }
        UserMap[userId].post(tweetId, timestamp++);
        
    }
    
    
    vector<int> getNewsFeed(int userId) {
       
        vector<int> feed;
        if(UserMap.find(userId) == UserMap.end()) {
            return feed;
        }
        
        // Create a priority queue of tweet heads to retrieve the most recent tweets.
        struct TweetComparator {
            bool operator()(const Tweet* t1, const Tweet* t2) const {
               return t1 -> time < t2 -> time;
            }
        };
        // Max by default in c++ ;)
        priority_queue<Tweet*, vector<Tweet*>, TweetComparator> pq;

        // Get all the followees of this user
        unordered_set<int> followees = UserMap[userId].follows;

        // Push 10 recent tweetheads of all these followees to get overall maximal  
        for(auto& followee : followees) {
            auto head = UserMap[followee].tweet_head;
            if(head != NULL) {
                pq.push(head);
            }            
        }
        int n = 10;
        // Now its time to get the required gems right up on the heap there!
        while(!pq.empty() && n > 0) {
            auto tweet = pq.top();
            pq.pop();
            feed.push_back(tweet -> id);
            n--;
            if(tweet -> next != NULL) {
                pq.push(tweet -> next);
            }
        }
        return feed;

    }
    
    void follow(int followerId, int followeeId) {
        if(UserMap.find(followerId) == UserMap.end()) {
            UserMap[followerId] = User(followerId);
        }
        if(UserMap.find(followeeId) == UserMap.end()) {
            UserMap[followeeId] = User(followeeId);
        }
       UserMap[followerId].follow(followeeId);
    }
    
    void unfollow(int followerId, int followeeId) {
       if(UserMap.find(followerId) != UserMap.end() && followerId != followeeId) {
           UserMap[followerId].unfollow(followeeId);
       }
       
    }

    
};

/**
 * Your Twitter object will be instantiated and called as such:
 * Twitter* obj = new Twitter();
 * obj->postTweet(userId,tweetId);
 * vector<int> param_2 = obj->getNewsFeed(userId);
 * obj->follow(followerId,followeeId);
 * obj->unfollow(followerId,followeeId);
 */

#include<bits/stdc++.h>
using namespace std;

class AiHw1{
    private:
        //common
        typedef pair<int, int> key;
        typedef vector<key> vectors;
        typedef unordered_map<int,vectors> tree_map;
        typedef unordered_map<int,stack<key> > frog_map;
        tree_map tree_colMap;
        tree_map tree_rowMap;
        tree_map tree_positiveDiag;
        tree_map tree_negativeDiag;
        vector<int> checkPossibilities;

        //dfs
        vectors placedFrogs;
        frog_map rowMap;
        frog_map positiveDiag;
        frog_map negativeDiag;

        //bfs
        struct node{
            vector< pair<int, int> > lists;
        };

        deque<node> qu;
        vector<pair<int,int> > result;
        bool flag=false;

        //sa
        unordered_map<int,vector<pair<pair<int,int>,int>>> ranges;
        unordered_map<int,vector<pair<pair<int,int>,int>>> ranges_neighbor;
        vectors current;
        vectors neighbor;
        vectors clashedSet;
        vectors clashedSet_neighbor;
        vector<int> freeRangesInColumn;
        vector<int> freeRangesInColumn_neighbor;
        vector<pair<int,pair<int,int>>> freeSlots;
        int thereAreFreeColumns=1;

    public:
        vector<pair<int,int> > trees;
        int n;
        string algo;
        long long int begin_time;
        long long int temperature;

        void initial_setup(){
            sort(trees.begin(), trees.end());
            for(vector<pair<int,int> >::iterator it=trees.begin(); it!=trees.end();it++){
                tree_colMap[ (*it).second ].push_back( (*it) );
                tree_rowMap[ (*it).first ].push_back( (*it) );
                tree_positiveDiag[ (*it).first + (*it).second ].push_back( (*it) );
                tree_negativeDiag[ (*it).first - (*it).second ].push_back( (*it) );
            }

            for(int j=0;j<n;j++){
                int counter=0;
                if( tree_colMap.find(j) != tree_colMap.end() ){
                    for(int i=0;i<n;i++){
                        if( find( trees.begin(),trees.end(),make_pair(i,j)) == trees.end() ){
                            counter++;
                            while(++i < n){
                                if( find( trees.begin(),trees.end(),make_pair(i,j)) != trees.end() )
                                    break;
                            }
                        }
                    }
                    checkPossibilities.push_back(counter);
                }
                else
                    checkPossibilities.push_back(1);
            }
            for(int j=n-2;j>=0;j--)
                checkPossibilities[j]+=checkPossibilities[j+1];
        }

        void write_to_file( vectors vect, bool success ){
            ofstream fout("output.txt");
            if(!success){
                fout<<"FAIL"<<'\n';
                return;
            }
            fout<<"OK";
            for(int i=0;i<n;i++){
                fout<<'\n';
                for(int j=0;j<n;j++){
                    if( find( trees.begin(),trees.end(),make_pair(i,j)) != trees.end() )
                        fout<<'2';
                    else if( find( vect.begin(),vect.end(),make_pair(i,j)) != vect.end() )
                        fout<<'1';
                    else 
                        fout<<'0';
                }
            }
            fout<<'\n';
        }

        bool noTreeInbetween_row(int i, int j){
            if( tree_rowMap.find(i) == tree_rowMap.end() )
                return true;
            else {
                for(vectors::iterator it=tree_rowMap[i].begin();it!=tree_rowMap[i].end();it++)
                    if( (*it).second < j && (*it).second > rowMap[i].top().second ) //can just have vector<int> instead of vector<key>
                        return false;
                return true;
            }
        }

        bool noTreeInbetween_pDiagnol(int i, int j){
            if( tree_positiveDiag.find(i+j) == tree_positiveDiag.end() )
                return true;
            else{
                for( vectors::iterator it=tree_positiveDiag[i+j].begin();it!=tree_positiveDiag[i+j].end();it++ )
                    if( (*it).second < j && (*it).second > positiveDiag[i+j].top().second && 
                            (*it).first > i && (*it).first < positiveDiag[i+j].top().first ) //can just have vector<int> instead of vector<key>
                        return false;
                return true;
            }
        }

        bool noTreeInbetween_nDiagnol(int i, int j){
            if( tree_negativeDiag.find(i-j) == tree_negativeDiag.end() )
                return true;
            else{
                for( vectors::iterator it=tree_negativeDiag[i-j].begin();it!=tree_negativeDiag[i-j].end();it++ )
                    if( (*it).second < j && (*it).second > negativeDiag[i-j].top().second && 
                            (*it).first < i && (*it).first > negativeDiag[i-j].top().first ) //can just have vector<int> instead of vector<key>
                        return false;
                return true;

            }
        }

        bool checker(int i, int j){
            if( rowMap.find(i)!=rowMap.end() && noTreeInbetween_row(i,j) )
                return false;
            if( positiveDiag.find(i+j)!=positiveDiag.end() && noTreeInbetween_pDiagnol(i,j) ) 
                return false;
            if( negativeDiag.find(i-j)!=negativeDiag.end() && noTreeInbetween_nDiagnol(i,j) )
                return false;

            return true;
        }

        void placeTheFrog(int i, int j){
            key k;
            k.first = i;
            k.second = j;
            placedFrogs.push_back(k);

            rowMap[i].push(k);

            positiveDiag[i+j].push(k);
            negativeDiag[i-j].push(k);
        }

        void removeTheFrog(int i, int j){
            key k;
            k.first=i;
            k.second=j;
            vectors::iterator index = find(placedFrogs.begin(), placedFrogs.end(), k);
            placedFrogs.erase(index);

            rowMap[i].pop();
            if(rowMap[i].size()==0)
                rowMap.erase(i);

            positiveDiag[i+j].pop();
            if(positiveDiag[i+j].size()==0)
                positiveDiag.erase(i+j);

            negativeDiag[i-j].pop();
            if(negativeDiag[i-j].size()==0)
                negativeDiag.erase(i-j);
        }

        int findNextIndex(int row, int col){

            if( tree_colMap.find(col) != tree_colMap.end() ){
                while(++row<n){
                    if( find(tree_colMap[col].begin(), tree_colMap[col].end(), make_pair(row, col)) != tree_colMap[col].end() )
                        break;
                }

                if( row == n || row == n-1 )
                    return -1;

                return row+1;
            }
            return -1;
        }

        bool solveFrogsUsingDFS( int row, int column, int frogsPlaced ){

            if( (time(NULL)-begin_time)  > 290.00 ){
                write_to_file(placedFrogs,false);
                exit(0);
            }

            placeTheFrog(row, column);

            frogsPlaced--;

            if(frogsPlaced==0)
                return true;

            int diff = findNextIndex(row, column);
            while (diff != -1) {
                int k = diff;
                while (  k < n )  {
                    if ( checker (k, column) && find( trees.begin(), trees.end(), make_pair(k,column)) == trees.end() ) {
                        if ( solveFrogsUsingDFS (k, column, frogsPlaced) ) {
                            return true;
                        }
                    }
                    k++;
                }
                diff = findNextIndex(diff, column);
            }

            for (int l = column + 1; l < n; ++l) {
                if( checkPossibilities[l] < frogsPlaced )
                    break;
                for (int k = 0; k < n; ++k) {
                    if ( ( find( trees.begin(),trees.end(),make_pair(k,l) ) == trees.end() ) && checker(k, l)) {
                        if (solveFrogsUsingDFS(k, l, frogsPlaced)) {
                            return true;
                        }
                    }
                }
            }
            frogsPlaced++;
            removeTheFrog(row, column);
            return false;
        }

        void itIsDFS(int frogs){

            initial_setup();
            if(checkPossibilities[0] < frogs){
                write_to_file(placedFrogs,false);
                return;
            }

            int start_row, start_column;

            for(start_column=0; start_column<n; start_column++){

                if( checkPossibilities[start_column] < frogs )
                    break;
                for(start_row=0; start_row<n; start_row++){
                    if( find(trees.begin(), trees.end(), make_pair(start_row, start_column) ) == trees.end() ){
                        if(solveFrogsUsingDFS(start_row,start_column,frogs)){
                            write_to_file(placedFrogs,true);
                            return; 
                        }
                    }
                }
            }
            write_to_file(placedFrogs,false);
        }

        bool isClash(int f_row, int f_col, int row, int col){

            if(f_row == row){
                int k;
                for(k=col-1;k>f_col;k--)
                    if( find( trees.begin(), trees.end(), make_pair(row, k) ) != trees.end() ) 
                        break;
                if(k==f_col)
                    return true;
            }

            // negative diag up
            if(f_row-f_col == row-col){
                int k=row-1, l=col-1;
                for(k,l;k>f_row && l>f_col;k--,l--)
                    if( find( trees.begin(), trees.end(), make_pair(k, l) ) != trees.end() ) 
                        break;
                if(k==f_row&&l==f_col)
                    return true;
            }

            // positive diag down
            if(f_row+f_col == row+col){
                int k=row+1, l=col-1;
                for(k,l;k<f_row && l>f_col;k++,l--)
                    if( find( trees.begin(), trees.end(), make_pair(k, l) ) != trees.end() ) 
                        break;
                if(k==f_row&&l==f_col)
                    return true;
            }
            return false;
        }

        bool checker_bfs(vector<pair<int,int> > placedFrogs, int row, int col){
            vector<pair<int, int> >::iterator it;
            for( it=placedFrogs.begin(); it!=placedFrogs.end();it++ ){
                if( isClash( (*it).first, (*it).second, row, col ) )
                    return false;
            }
            return true;
        }

        bool nodesInNextColumn(vector<pair<int,int>> placedFrogs, int row, int col, int frogsPlaced){

            for(int i=0;i<n;i++){
                if( find( trees.begin(), trees.end(), make_pair(i, col) ) == trees.end() && checker_bfs(placedFrogs,i,col) ) {
                    flag=true;
                    node value;
                    value.lists = placedFrogs;
                    value.lists.push_back( make_pair(i, col) );
                    qu.push_back(value);
                    if(value.lists.size()==frogsPlaced){
                        result = value.lists;
                        //clear the queue
                        qu.clear();
                        return true;
                    }
                }
            }
            return false;
        }

        bool nodesInSameColumn( vector<pair<int,int>> placedFrogs, int row, int col, int frogsPlaced ){
            if( tree_colMap.find(col) != tree_colMap.end() ){

                while(++row<n){
                    if( find(tree_colMap[col].begin(), tree_colMap[col].end(), make_pair(row, col)) != tree_colMap[col].end() )
                        break;
                }

                if(row==n || row==n-1)
                    return false;

                for( int k=row+1;k<n;k++ ) 
                    if( find(trees.begin(), trees.end(), make_pair(k, col)) == trees.end() && checker_bfs(placedFrogs,k,col) ) {
                        node value;
                        value.lists = placedFrogs;
                        value.lists.push_back( make_pair(k, col) );
                        qu.push_back(value);
                        if(value.lists.size()==frogsPlaced){
                            result = value.lists;
                            qu.clear();
                            return true;
                        }
                    }
            }
            return false;
        }

        bool solveFrogsUsingBFS( int frogsPlaced ){
            while( !qu.empty() ) {

                if( (time(NULL)-begin_time) > 290.00 ){
                    write_to_file(result,false);
                    exit(0);
                }

                node topVal = qu.front();

                if(topVal.lists.size()==frogsPlaced){
                    result = topVal.lists;
                    qu.clear();
                    return true;
                }
                qu.pop_front();

                if(nodesInSameColumn( topVal.lists, topVal.lists.back().first, topVal.lists.back().second, frogsPlaced ))
                    return true;
                for(int j=topVal.lists.back().second+1; j<n; j++){
                    if( tree_colMap.find(j)!=tree_colMap.end() && tree_colMap[j].size()==n )
                        continue;

                    if( nodesInNextColumn( topVal.lists, topVal.lists.back().first,j,frogsPlaced ) )
                        return true;
                    if(flag){
                        flag=false;
                        break;
                    }
                }
            }
            return false;
        }
        void itIsBFS(int frogs){

            initial_setup();
            if(checkPossibilities[0] < frogs){
                write_to_file(result,false);
                return;
            }
            int start_row, start_column;

            for(start_column=0; start_column<n; start_column++){

                if( checkPossibilities[start_column] < frogs )
                    break;


                for(start_row=0; start_row<n; start_row++){
                    if( find(trees.begin(), trees.end(), make_pair(start_row, start_column) ) == trees.end() ){
                        node value;
                        value.lists.push_back( make_pair(start_row, start_column) );
                        qu.push_back(value);
                    }
                }
                if( solveFrogsUsingBFS ( frogs ) ){

                    write_to_file(result,true);
                    return; 
                }
            }
            write_to_file(result, false);
        } 

        void splitIntoRanges(){
            for(int j=0;j<n;j++){
                if( tree_colMap.find(j) != tree_colMap.end() ){
                    int start=0;
                    int i=0;

                    while(i<n){
                        if(find( trees.begin(),trees.end(),make_pair(i,j)) == trees.end()){
                            start=i;
                            while(i<n){
                                i++;
                                if(find( trees.begin(),trees.end(),make_pair(i,j)) != trees.end())
                                    break;
                            }
                            ranges[j].push_back( make_pair(make_pair(start,i-1),1) );
                        } else {
                            while(i<n){
                                i++;
                                if(find( trees.begin(),trees.end(),make_pair(i,j)) == trees.end())
                                    break;
                            }
                        }
                    }
                }
                else
                    ranges[j].push_back( make_pair(make_pair(0,n-1),1) );
            }
        }

        void initial_setup_sa(){
            sort(trees.begin(), trees.end());
            for(vector<pair<int,int>>::iterator it=trees.begin(); it!=trees.end();it++){
                tree_colMap[ (*it).second ].push_back( (*it) );
                tree_rowMap[ (*it).first ].push_back( (*it) );
                tree_positiveDiag[ (*it).first + (*it).second ].push_back( (*it) );
                tree_negativeDiag[ (*it).first - (*it).second ].push_back( (*it) );
            }

            splitIntoRanges();
            freeRangesInColumn.assign(n,1);
        }

        int clashForEachQueens(vectors vect, int row, int col, bool isNeighbor){

            // row back
            int clashes=0;
            for(int j=col-1;j>=0;j--){
                if( find(trees.begin(), trees.end(), make_pair(row,j))!=trees.end() ) 
                    break;

                if( find(vect.begin(),vect.end(),make_pair(row,j))!=vect.end() ){
                    clashes++;
                    break;
                }
            }
            // row front
            for(int j=col+1;j<n;j++){
                if( find(trees.begin(), trees.end(), make_pair(row,j))!=trees.end() ) 
                    break;

                if( find(vect.begin(),vect.end(),make_pair(row,j))!=vect.end() ){
                    clashes++;
                    break;
                }
            }

            //column up
            for(int i=row-1;i>=0;i--){
                if( find(trees.begin(), trees.end(), make_pair(i,col))!=trees.end() ) 
                    break;

                if( find(vect.begin(),vect.end(),make_pair(i,col))!=vect.end() ){
                    clashes++;
                    break;
                }
            }

            //column down
            for(int i=row+1;i<n;i++){
                if( find(trees.begin(), trees.end(), make_pair(i,col))!=trees.end() ) 
                    break;

                if( find(vect.begin(),vect.end(),make_pair(i,col))!=vect.end() ){
                    clashes++;
                    break;
                }
            }

            //left top
            for(int i=row-1, j=col-1;i>=0&&j>=0;i--,j--){
                if( find(trees.begin(), trees.end(), make_pair(i,j))!=trees.end() ) 
                    break;

                if( find(vect.begin(),vect.end(),make_pair(i,j))!=vect.end() ){
                    clashes++;
                    break;
                }

            }

            //left down
            for(int i=row+1,j=col-1; i<n&&j>=0;i++,j--){
                if( find(trees.begin(), trees.end(), make_pair(i,j))!=trees.end() ) 
                    break;

                if( find(vect.begin(),vect.end(),make_pair(i,j))!=vect.end() ){
                    clashes++;
                    break;
                }
            }
            //right top
            for(int i=row-1,j=col+1; i>=0&&j<n;i--,j++){
                if( find(trees.begin(), trees.end(), make_pair(i,j))!=trees.end() ) 
                    break;

                if( find(vect.begin(),vect.end(),make_pair(i,j))!=vect.end() ){
                    clashes++;
                    break;
                }
            }
            //right down
            for(int i=row+1,j=col+1; i<n&&j<n;i++,j++){
                if( find(trees.begin(), trees.end(), make_pair(i,j))!=trees.end() ) 
                    break;

                if( find(vect.begin(),vect.end(),make_pair(i,j))!=vect.end() ){
                    clashes++;
                    break;
                }
            }
            if(clashes>0){
                if(isNeighbor)
                    clashedSet_neighbor.push_back(make_pair(row,col));
                else
                    clashedSet.push_back(make_pair(row,col));
            }
            return clashes;
        }

        int calculateCost(vectors vect, bool isNeighbor){
            vectors::iterator it;
            int total_clashes=0;

            if(isNeighbor)
                clashedSet_neighbor.clear();
            else
                clashedSet.clear();

            for(it=vect.begin();it!=vect.end();it++)
                total_clashes += clashForEachQueens( vect, (*it).first, (*it).second, isNeighbor );

            return total_clashes;
        }



        void setUpFreeSlots(){

            for(int j=0;j<n;j++){
                if(freeRangesInColumn_neighbor[j]==1){
                    for(vector<pair<pair<int,int>,int>>::iterator it=ranges_neighbor[j].begin(); it!=ranges_neighbor[j].end();it++){
                        if( (*it).second == 1 ){
                            freeSlots.push_back( make_pair( j, make_pair( (*it).first.first,(*it).first.second ) ) );
                        }
                    }
                }
            }
        }


        bool flipTheCoin(){
            double nr = temperature - time(NULL);
            double dr = temperature - begin_time;
            double val = (nr/dr) * 100;
            int flip = rand()%101;
            if(val<flip)
                return false;
            return true;

        }
        bool generateInitialRandomSolution(int frogs){

            int count = 0, flag=0;
            unordered_map<int,vector<pair<pair<int,int>,int>>>::iterator index;
            vector<pair<pair<int,int>,int>>::iterator ii;

            for(index=ranges.begin(); index!=ranges.end();index++){
                for(ii=(*index).second.begin(); ii!=(*index).second.end();ii++){
                    int range = (*ii).first.second-(*ii).first.first+1;
                    int num = rand()%range + (*ii).first.first;
                    (*ii).second=0;
                    current.push_back( make_pair( num, (*index).first ) );
                    if(++count==frogs){
                        flag=1;
                        break;
                    }
                }
                if(flag){
                    ii++;
                    if( ii==(*index).second.end() )
                        freeRangesInColumn[ (*index).first ] = 0;
                    break;
                }
                freeRangesInColumn[ (*index).first ] = 0;
            }
            while(count<frogs)
                return false;

            if( ++index==ranges.end() )
                thereAreFreeColumns=0;

            return true;
        }

        void generateNeighbor(){

            clashedSet_neighbor.clear();
            neighbor.clear();
            freeRangesInColumn_neighbor.clear();
            ranges_neighbor.clear();

            neighbor.assign(current.begin(),current.end());
            freeRangesInColumn_neighbor.assign(freeRangesInColumn.begin(),freeRangesInColumn.end());
            ranges_neighbor = ranges;
            clashedSet_neighbor.assign(clashedSet.begin(),clashedSet.end());


            if(thereAreFreeColumns){
                setUpFreeSlots();

                //remove the clashing queen
                int pickAQueen = rand()%clashedSet_neighbor.size();
                pair<int,int> pa = clashedSet_neighbor[pickAQueen];
                int count=0;
                for(vector<pair<pair<int, int>,int>>::iterator it=ranges_neighbor[pa.second].begin();it<=ranges_neighbor[pa.second].end();it++){
                    if( pa.first>=(*it).first.first && pa.first<=(*it).first.second )
                        break;
                    count++;
                }
                vectors::iterator index = find(neighbor.begin(),neighbor.end(),pa);
                if(index!= neighbor.end()){
                    neighbor.erase(index);
                }
                index = find(clashedSet_neighbor.begin(),clashedSet_neighbor.end(),pa);
                if(index!= neighbor.end()){
                    clashedSet_neighbor.erase(index);
                }

                //insert a queen at a random position in a free slot
                int index1 = rand()%freeSlots.size();
                pair<int,pair<int,int>> aa = freeSlots[index1];

                int maxmin = aa.second.second-aa.second.first+1;
                int num = (rand()%maxmin) + aa.second.first;

                neighbor.push_back( make_pair(num,aa.first) );

                // corresponding changes
                vector<pair<pair<int,int>,int>>::iterator it = find(ranges_neighbor[aa.first].begin(),ranges_neighbor[aa.first].end(),make_pair(make_pair(aa.second.first,aa.second.second),1));
                if(it != ranges_neighbor[aa.first].end())
                    (*it).second = 0;

                int flag=1;
                for( it=ranges_neighbor[aa.first].begin();it!=ranges_neighbor[aa.first].end();it++ )
                    if( (*it).second==1 ){
                        flag=0;
                        break;
                    }
                if(!flag)
                    freeRangesInColumn_neighbor[aa.first]=1;
                else
                    freeRangesInColumn_neighbor[aa.first]=0;

                ranges_neighbor[pa.second][count].second=1;
                freeRangesInColumn_neighbor[pa.second]=1;
                freeSlots.clear(); 
            }
            else{
                //move the clashed queen inside a same range
                int pickAQueen = rand()%clashedSet_neighbor.size();
                pair<int,int> pa = clashedSet_neighbor[pickAQueen];
                int count=0;
                for(vector<pair<pair<int, int>,int>>::iterator it=ranges_neighbor[pa.second].begin();it<=ranges_neighbor[pa.second].end();it++){
                    if( pa.first>=(*it).first.first && pa.first<=(*it).first.second )
                        break;
                    count++;
                }

                vectors::iterator index = find(neighbor.begin(),neighbor.end(),pa);
                if(index!= neighbor.end()){
                    neighbor.erase(index);
                }
                index = find(clashedSet_neighbor.begin(),clashedSet_neighbor.end(),pa);
                if(index!= neighbor.end()){
                    clashedSet_neighbor.erase(index);
                }

                int maxmin = ranges_neighbor[pa.second][count].first.second-ranges_neighbor[pa.second][count].first.first+1;
                int num = rand()%maxmin + ranges_neighbor[pa.second][count].first.first;

                neighbor.push_back(make_pair(num,pa.second));
            }
        }


        bool itIsSA(int frogs){

            initial_setup_sa();
            if(!generateInitialRandomSolution(frogs)){
                write_to_file(current,false);
                return false;
            }

            while( time(NULL) < temperature ){
                
                int curr = calculateCost(current,false);
                if(curr==0){
                    write_to_file(current, true);
                    return true;
                }

                generateNeighbor();
                int neigh = calculateCost(neighbor,true);
                if(neigh==0){
                    write_to_file(neighbor,true);
                    return true;
                }

                if(neigh<curr){
                    current.clear();
                    freeRangesInColumn.clear();
                    ranges.clear();
                    clashedSet.clear();

                    current=neighbor;
                    freeRangesInColumn=freeRangesInColumn_neighbor;
                    ranges=ranges_neighbor;
                    clashedSet=clashedSet_neighbor;
                } else{
                    if(!flipTheCoin()){
                        current.clear();
                        freeRangesInColumn.clear();
                        ranges.clear();
                        clashedSet.clear();

                        current=neighbor;
                        freeRangesInColumn=freeRangesInColumn_neighbor;
                        ranges=ranges_neighbor;
                        clashedSet=clashedSet_neighbor;
                    }
                }
            }
            write_to_file(current,false);
            return false; 
        }

        void start(int frogs){

            tree_colMap.clear();
            tree_rowMap.clear();
            tree_positiveDiag.clear();
            tree_negativeDiag.clear();
            rowMap.clear();
            positiveDiag.clear();
            negativeDiag.clear();
            placedFrogs.clear();
            result.clear();
            qu.clear();

            if(algo.compare("DFS")==0)
                itIsDFS(frogs);
            else if(algo.compare("BFS")==0)
                itIsBFS(frogs);
            else if(algo.compare("SA")==0){
                srand(time(NULL));
                itIsSA(frogs);
            }
        }
};

int main(){
    AiHw1 obj;
    obj.begin_time = time(NULL);

    obj.temperature = obj.begin_time + 4.7*60;

    ifstream fin("input.txt");
    string str;
    int mode, frogs = 0;

    getline(fin, obj.algo);

    getline(fin, str);
    stringstream str1(str);
    str1 >> obj.n;

    getline(fin, str);
    stringstream str2(str);
    str2 >> frogs;

    for(int i=0;i<obj.n;i++){
        getline(fin, str);
        for(int j=0;j<str.length();j++)
            if( str[j] == '2' ){
                pair<int, int> pa;
                pa=make_pair(i,j);
                obj.trees.push_back(pa);
            }
    }
    obj.start(frogs);
    return 0;
}

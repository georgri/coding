#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    // Read init information from standard input, if any
	int N, L, E;
	scanf("%d %d %d", &N, &L, &E);
	
	NSMutableArray *links = [[NSMutableArray alloc] initWithCapacity:N];
	// Fill the cells with empty arrays.
	for (int i = 0; i < N; i++) {
		[links addObject:[[NSMutableIndexSet alloc] init]];
	}
	for (int i = 0; i < L; ++i) {
	    int from, to;
	    scanf("%d %d", &from, &to);
	    [[links objectAtIndex:from] addIndex:to];
	    [[links objectAtIndex:to]   addIndex:from];
	}
	
	// Initialize an array with only links adjacent to gates
	NSMutableArray *gates = [[NSMutableArray alloc] initWithCapacity:N];
	for(int i = 0; i < N; ++i) {
	    [gates addObject:[[NSMutableIndexSet alloc] init]];
	}
	// So awfull code... I can't imagine people write like this all their lives.
	for (int i = 0; i < E; ++i) {
	    int exit;
	    scanf("%d", &exit);

    	// For every exit add all the adjacent links to the gates array
    	[[links objectAtIndex:exit] enumerateIndexesUsingBlock:^(NSUInteger idx, BOOL *stop) {
    	   [[gates objectAtIndex:idx] addIndex:exit];
    	}];
	}
	
    



    while (1) {
        // Read information from standard input
        int n;
        scanf("%d", &n);
        
        // Edge to remove at the end of the day
        __block int removeFrom, removeTo;
        
        // Compute logic here
        // Check if exit is adjacent
        NSMutableIndexSet *adjExits = [gates objectAtIndex:n];
        if ([adjExits count] > 0) {
            // Just remove the gate edge
            removeFrom = n;
            removeTo = [adjExits firstIndex];
        } else {
            // Queue of edges
            __block NSMutableArray *queue = [[NSMutableArray alloc] init];
            [queue addObject:[[NSNumber alloc] initWithInt:n]];
            
            // Array of visited vertices
            __block NSMutableIndexSet *visited = [[NSMutableIndexSet alloc] init];
            [visited addIndex:n];

            while ([queue count] > 0) {
                // Pop the first element
                int cur = [[queue objectAtIndex:0] intValue];
                [queue removeObjectAtIndex:0];
                
                // Add adjacent vertices with 0 distances to the queue
                [[links objectAtIndex:cur] enumerateIndexesUsingBlock:^(NSUInteger idx, BOOL *stop) {
                    if ([visited containsIndex:idx] == NO) {
                        [visited addIndex:idx];
                        if ([[gates objectAtIndex:idx] count] > 0) {
                            removeFrom = idx;
                            removeTo = [[gates objectAtIndex:idx] firstIndex];
                            // Same distance => push in front of a queue!
                            [queue insertObject:[[NSNumber alloc] initWithInt:idx] atIndex:0];
                            // If the vertex is double-gated, return it immediately
                            if ([[gates objectAtIndex:idx] count] >= 2) {
                                [queue removeAllObjects]; // stop the while loop
                                *stop = YES;
                            }
                        } else {
                            [queue addObject:[[NSNumber alloc] initWithInt:idx]];
                        }
                    }
                }];
            }
        }
        
        // Removing link from an array of links
        [[gates objectAtIndex:removeFrom] removeIndex:removeTo];
        [[links objectAtIndex:removeFrom] removeIndex:removeTo];
        [[links objectAtIndex:removeTo]   removeIndex:removeFrom];
        
        // Write action to standard output
        printf("%d %d\n", removeFrom, removeTo);
    }

    return 0;
}
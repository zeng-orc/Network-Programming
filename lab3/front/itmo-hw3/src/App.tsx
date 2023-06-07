import React,{Component} from 'react'
import CssBaseline from '@material-ui/core/CssBaseline';
import Container from '@material-ui/core/Container';
import Board from './Board'
import Comment from './Comment'
import List from '@material-ui/core/List';
import axios from 'axios';

// interface templist
// {
//     [list:string]:string,
//     addList:string,
//     child:any
//     addList:(newAuthor:any,newComment:any) =>any
// }

class App extends Component<any,any>{
    constructor(props:any){
        super(props) 
        this.state={
            list:[]
        }
      
    }
    componentDidMount(){
        this.query();
    }
    render(){
        return(
            <React.Fragment>
                <CssBaseline />
                    <Container maxWidth="sm">
                        <Comment 
                        addList={this.addList.bind(this)}/>
                        <List>
                        {
                            this.state.list.map((item:any,index:any)=>{
                                return (
                                     <Board 
                                        key = {item.author+index}
                                        author ={item.author} 
                                        comment = {item.content}
                                    />
                                )
                            })
                        }
                        </List>
                    </Container>
          </React.Fragment>
        )
    }
    query = () =>{
        axios.get('/api/getComments').then(({data})=>{
            this.setState({
                list:data
            })
        });
    }

    // onRef = (ref:any) => {
    //     //this.child = ref
    // }

    addList(newAuthor:any,newComment:any){
        console.log(newAuthor)
        console.log(newComment)
        axios.post(`/api/addComment`,{
            author : newAuthor,
            content : newComment
        }).then(({data}) => {
            console.log(newAuthor)
            this.query()
            alert(data)
        })

    }
}

export default App;